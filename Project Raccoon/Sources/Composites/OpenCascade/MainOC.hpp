#pragma once

#include "Common.hpp"
#include "RenderedOC.hpp"





struct MainOC
{

    Shader& shader3D;
    Camera& camera;
    GlobalVariables& gv;

    Sphere light;
    p3 lightPos = { 30,25,40 };
    std::array<float, 16> model3DMatrix = gv.identityMatrix;

    Lines3D lines;
    Polyhedra polygon;
    
    ShapeRenderer render1;
    

	MainOC(Shader& shader3D_, Camera& camera_, GlobalVariables& gv_) 
        :shader3D(shader3D_), camera(camera_), gv(gv_), light(3), render1(gv, shader3D)
	{ 
        

        render1.addBoxShape({ 0, 0, 0 }, { 5, 3, 2 });
        render1.addSphereShape({ 0, 0, 0 }, 5);
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
        shader3D.setUniform("u_Model", gv.identityMatrix);
        shader3D.setUniform("u_Color", 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1);

        shader3D.setUniform("u_fragmentMode", 0);
        //light.draw();


        render1.draw();

        opaque();
    }
};