#pragma once
#include "Common.hpp"
#include "IncludesOC.hpp"



void extractFaceData(const TopoDS_Face& face, std::vector<p3>& positions, std::vector<unsigned int>& trIndices, std::vector<unsigned int>& wireIndices
	, std::vector<p3>& normals)
{
	unsigned int indexOffset = positions.size(); //Works bc a face won't have a repeated vertex

	BRepMesh_IncrementalMesh mesh(face, 0.01);
	TopLoc_Location loc;
	Handle(Poly_Triangulation) triangulation = BRep_Tool::Triangulation(face, loc);
	if (triangulation.IsNull()) return;

	const gp_Trsf& trsf = loc.Transformation();

	//positions
	const int nbNodes = triangulation->NbNodes();
	positions.reserve(trIndices.size() + nbNodes);

	for (int i = 1; i <= nbNodes; ++i)
	{
		const gp_Pnt pnt = triangulation->Node(i).Transformed(trsf);
		const p3 pt{ static_cast<float>(pnt.X()), static_cast<float>(pnt.Y()), static_cast<float>(pnt.Z()) };
		positions.insert(positions.end(), pt);
	}

	//trIndices
	const int nbTris = triangulation->NbTriangles();
	trIndices.reserve(trIndices.size() + nbTris * 3);
	normals.reserve(normals.size() + nbTris * 3);

	for (int i = 1; i <= nbTris; ++i)
	{
		int n1, n2, n3;
		triangulation->Triangle(i).Get(n1, n2, n3);
		trIndices.insert(trIndices.end(), {
			static_cast<unsigned int>(indexOffset + n1 - 1),
			static_cast<unsigned int>(indexOffset + n2 - 1),
			static_cast<unsigned int>(indexOffset + n3 - 1)
			});

		//normals
		const p3& v1 = positions[n1 - 1];
		const p3& v2 = positions[n2 - 1];
		const p3& v3 = positions[n3 - 1];
		const p3 faceNormal = normalize3(cross3(v2 - v1, v3 - v1));
		normals.insert(normals.end(), { faceNormal, faceNormal, faceNormal });
	}


	//wire indices
	trIndices.reserve(trIndices.size() * 2);
	for (unsigned int i = 0; i < trIndices.size(); i += 3)
	{
		wireIndices.insert(wireIndices.end(),
			{
				trIndices[i],trIndices[i + 1],
				trIndices[i + 1],trIndices[i + 2],
				trIndices[i + 2],trIndices[i]
			});
	}
}


void makeRectangle(p3 corner1, p3 corner2, p3 planeNormal, TopoDS_Wire& squareWire, TopoDS_Face& squareFace)
{
	gp_Pnt A(corner1.x, corner1.y, corner1.z);
	gp_Pnt C(corner2.x, corner2.y, corner2.z);
	gp_Vec AC(A, C);
	gp_Dir N(planeNormal.x, planeNormal.y, planeNormal.z);

	// Step 1: Choose one axis perpendicular to normal
	gp_Vec U = (std::abs(N.X()) < 0.9) ? gp_Vec(1, 0, 0) : gp_Vec(0, 1, 0);
	gp_Vec X = N ^ U; // X axis in the plane
	gp_Vec Y = N ^ X; // Y axis in the plane
	X.Normalize();
	Y.Normalize();

	// Step 2: Project AC onto local axes
	double xLen = AC * X;
	double yLen = AC * Y;

	gp_Pnt B = A.Translated(X * xLen);
	gp_Pnt D = A.Translated(Y * yLen);

	// Debug print
	/*std::cout << "A: (" << A.X() << ", " << A.Y() << ", " << A.Z() << ")\n";
	std::cout << "B: (" << B.X() << ", " << B.Y() << ", " << B.Z() << ")\n";
	std::cout << "C: (" << C.X() << ", " << C.Y() << ", " << C.Z() << ")\n";
	std::cout << "D: (" << D.X() << ", " << D.Y() << ", " << D.Z() << ")\n";*/

	// Create edges A-B-C-D-A
	TopoDS_Edge edge1 = BRepBuilderAPI_MakeEdge(A, B);
	TopoDS_Edge edge2 = BRepBuilderAPI_MakeEdge(B, C);
	TopoDS_Edge edge3 = BRepBuilderAPI_MakeEdge(C, D);
	TopoDS_Edge edge4 = BRepBuilderAPI_MakeEdge(D, A);

	squareWire = BRepBuilderAPI_MakeWire(edge1, edge2, edge3, edge4);
	squareFace = BRepBuilderAPI_MakeFace(squareWire);
}
