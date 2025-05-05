#pragma once

#include "Common.hpp"
#include "RenderedOC.hpp"




void debugDrawBox(const p3& corner, const p3& size)
{
    gp_Pnt gpCorner(corner.x, corner.y, corner.z);

    TopoDS_Shape box = BRepPrimAPI_MakeBox
    (
        gpCorner,
        static_cast<Standard_Real>(size.x),
        static_cast<Standard_Real>(size.y),
        static_cast<Standard_Real>(size.z)
    );

    std::vector<p3> trPositions, trNormals, wirePositions;
    std::vector<unsigned int> trIndices, wireIndices;

    Polyhedra polygon;
    Lines3D lines;

    unsigned int trIndexOffset = 0;
    unsigned int wireIndexOffset = 0;
    for (TopExp_Explorer exp(box, TopAbs_FACE); exp.More(); exp.Next()) 
    {
        const TopoDS_Face& face = TopoDS::Face(exp.Current());

        std::vector<p3> interm_trPositions, interm_trNormals, interm_wirePositions;
        std::vector<unsigned int> interm_trIndices, interm_wireIndices;

        
        

       
        extractFaceData(face, interm_trPositions, interm_trIndices, interm_trNormals, interm_wirePositions, interm_wireIndices
            , trIndexOffset, wireIndexOffset);

        trPositions.insert(trPositions.end(), interm_trPositions.begin(), interm_trPositions.end());
        trIndices.insert(trIndices.end(), interm_trIndices.begin(), interm_trIndices.end());
        trNormals.insert(trNormals.end(), interm_trNormals.begin(), interm_trNormals.end());

        wirePositions.insert(wirePositions.end(), interm_wirePositions.begin(), interm_wirePositions.end());
        wireIndices.insert(wireIndices.end(), interm_wireIndices.begin(), interm_wireIndices.end());

        

        trIndexOffset = trPositions.size(); //Works bc a face won't have a repeated vertex
        wireIndexOffset = wirePositions.size();

        print(wirePositions.size());
        print(wirePositions);
        print(wireIndices);
        break;
    }

    print("aaaaaaaaaaaaaaaaaa");
    lines.addSet(wirePositions, 1);
    lines.indices = wireIndices;
    lines.isBufferUpdated = 1;

    
    
    polygon.addPolyhedra(trPositions, trIndices, trNormals);
    //polygon.draw();
    glLineWidth(3);
    lines.draw();
    glLineWidth(1);

}

struct MainOC
{

    Shader& shader3D;
    Camera& camera;
    Sphere light;
    p3 lightPos = { 30,25,40 };
    std::array<float, 16> model3DMatrix = camera.identityMatrix;

    Lines3D lines;
    Polyhedra polygon;
    
    ShapeRenderer algo;

	MainOC(Shader& shader3D_, Camera& camera_, GlobalVariables gv) :shader3D(shader3D_), camera(camera_), light(3)
	{
		


		/*TopoDS_Wire wire;
		TopoDS_Face face;
        makeRectangle({ 0,0,0 }, { 10,0,10 }, { 0,1,0 }, wire, face);
        
        

        std::vector<p3> positions, normals;
        std::vector<unsigned int> indices;
        std::vector<p3> linePositions;
        std::vector<unsigned int> lineIndices;

        extractFaceData(face, positions, indices, normals,linePositions);


        lines.addSet(linePositions);

        polygon.addPolyhedra(positions, indices, normals);*/

        

        light.addSet(lightPos);
        if (gv.program == telemetry)
        {
            shader3D.bind();
            shader3D.setUniform("u_lightPos", lightPos);
        }
	}

    void draw()
    {
        shader3D.bind();
        shader3D.setUniform("u_Model", camera.identityMatrix);
        shader3D.setUniform("u_fragmentMode", 1);
        light.draw();

        shader3D.setUniform("u_Color", 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1);
        //shader3D.setUniform("u_Color", 40.0f / 255.0f, 239.9f / 255.0f, 239.0f / 255.0f, 1);
        /*lines.draw();
        polygon.draw();*/
        
        p3 corner = { 0, 0, 0 };
        p3 size = { 5, 3, 2 };
        debugDrawBox(corner, size);

        //algo.draw();

        opaque();
    }
};