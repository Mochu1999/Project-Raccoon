#pragma once
#include "Common.hpp"
#include "IncludesOC.hpp"

void extractFaceData(const TopoDS_Face& face, std::vector<p3>& trPositions, std::vector<unsigned int>& trIndices, std::vector<p3>& normals
    , std::vector<p3>& wirePositions)
{
    BRepMesh_IncrementalMesh mesh(face, 0.01);
    TopLoc_Location loc;
    Handle(Poly_Triangulation) triangulation = BRep_Tool::Triangulation(face, loc);
    if (triangulation.IsNull()) return;

    const gp_Trsf& trsf = loc.Transformation();

    const int nbNodes = triangulation->NbNodes();
    trPositions.clear();
    wirePositions.clear();
    trPositions.reserve(nbNodes);
    wirePositions.reserve(nbNodes + 1);

    for (int i = 1; i <= nbNodes; ++i) {
        const gp_Pnt pnt = triangulation->Node(i).Transformed(trsf);
        const p3 pt{ static_cast<float>(pnt.X()), static_cast<float>(pnt.Y()), static_cast<float>(pnt.Z()) };
        trPositions.insert(trPositions.end(), pt);
        wirePositions.insert(wirePositions.end(), pt);
    }
    wirePositions.push_back(wirePositions.front());

    const int nbTris = triangulation->NbTriangles();
    trIndices.clear();
    trIndices.reserve(nbTris * 3);
    normals.clear();
    normals.reserve(nbTris * 3);

    for (int i = 1; i <= nbTris; ++i) {
        int n1, n2, n3;
        triangulation->Triangle(i).Get(n1, n2, n3);
        trIndices.insert(trIndices.end(), {
            static_cast<unsigned int>(n1 - 1),
            static_cast<unsigned int>(n2 - 1),
            static_cast<unsigned int>(n3 - 1)
            });

        const p3& v1 = trPositions[n1 - 1];
        const p3& v2 = trPositions[n2 - 1];
        const p3& v3 = trPositions[n3 - 1];
        const p3 faceNormal = normalize3(cross3(v2 - v1, v3 - v1));
        normals.insert(normals.end(), { faceNormal, faceNormal, faceNormal });
    }
}

void extractFaceData(const TopoDS_Face& face, std::vector<p3>& trPositions, std::vector<unsigned int>& trIndices, std::vector<p3>& normals
    , std::vector<p3>& wirePositions, std::vector<unsigned int>& wireIndices, unsigned int& trIndexOffset, unsigned int& wireIndexOffset)
{
    BRepMesh_IncrementalMesh mesh(face, 0.01);
    TopLoc_Location loc;
    Handle(Poly_Triangulation) triangulation = BRep_Tool::Triangulation(face, loc);
    if (triangulation.IsNull()) return;

    const gp_Trsf& trsf = loc.Transformation();

    const int nbNodes = triangulation->NbNodes();
    trPositions.clear();
    wirePositions.clear();
    trPositions.reserve(nbNodes);
    wirePositions.reserve(nbNodes + 1);

    for (int i = 1; i <= nbNodes; ++i) {
        const gp_Pnt pnt = triangulation->Node(i).Transformed(trsf);
        const p3 pt{ static_cast<float>(pnt.X()), static_cast<float>(pnt.Y()), static_cast<float>(pnt.Z()) };
        trPositions.insert(trPositions.end(), pt);
        wirePositions.insert(wirePositions.end(), pt);
    }
    wirePositions.push_back(wirePositions.front());

    const int nbTris = triangulation->NbTriangles();
    trIndices.clear();
    trIndices.reserve(nbTris * 3);
    normals.clear();
    normals.reserve(nbTris * 3);

    for (int i = 1; i <= nbTris; ++i) {
        int n1, n2, n3;
        triangulation->Triangle(i).Get(n1, n2, n3);
        trIndices.insert(trIndices.end(), {
            static_cast<unsigned int>(trIndexOffset + n1 - 1),
            static_cast<unsigned int>(trIndexOffset + n2 - 1),
            static_cast<unsigned int>(trIndexOffset + n3 - 1)
            });

        const p3& v1 = trPositions[n1 - 1];
        const p3& v2 = trPositions[n2 - 1];
        const p3& v3 = trPositions[n3 - 1];
        const p3 faceNormal = normalize3(cross3(v2 - v1, v3 - v1));
        normals.insert(normals.end(), { faceNormal, faceNormal, faceNormal });
    }
    

    //wire indices
    for (unsigned int i = 0; i < wirePositions.size() - 1; i++)
    {
        wireIndices.insert(wireIndices.end(), { wireIndexOffset + i,wireIndexOffset + i + 1 });
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
