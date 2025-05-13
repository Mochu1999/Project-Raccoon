#pragma once

#include "Common.hpp"
#include "ShapeRenderer.hpp"
#include "axis.hpp"
#include "CadCreation.hpp"
#include "Text.hpp"


struct MainOC
{
	Shader& shader3D;
	Shader& shaderText;
	Camera& camera;
	GlobalVariables& gv;

	Text text;
	CadCreation cadCreator;

	Axis axis;
	AxisOCC axisOCC;


	Sphere light;
	p3 lightPos = { 300,250,40 };
	matrix4x4 model3DMatrix = gv.identityMatrix;

	Lines3D lines;
	Polyhedra polygon;

	ShapeRenderer a, b, c;
	

	MainOC(Shader& shader3D_, Shader& shaderText_, Camera& camera_, GlobalVariables& gv_)
		:shader3D(shader3D_),shaderText(shaderText_), camera(camera_), gv(gv_), light(3)
		, a(gv, shader3D), b(gv, shader3D), c(gv, shader3D)
		, axisOCC(shader3D, gv, gv.identityMatrix), axis(shader3D, gv),
		cadCreator(shader3D,camera, gv, a), text("resources/Glyphs/Helvetica/Helvetica.otf", 36)
	{
		a.importStlOpenCascade();
		/*a.addRectangle({ 0,0,0 }, { 5,0,5 });
		a.extrudeFace(10);
		b.addSphereShape({ 5,0,5 }, 8);
		TopoDS_Shape intermShape;*/
		//booleanUnion(c, a, b);
		//booleanIntersection(c, a, b);
		//booleanCut(c, a, b);



		//a.addCircle({ 0,0,0 }, 0);
		//a.addBoxShape({ 0, 0, 0 }, { 5, 3, 2 });
		//a.addSphereShape({ 0, 0, 0 }, 5);
		//a.addIGES("ingenaval.igs");
		light.addSet(lightPos);

		if (gv.program == telemetry)
		{
			shader3D.bind();
			shader3D.setUniform("u_lightPos", lightPos);
		}
	}

	void draw()
	{
		cadCreator.draw();
		



		shader3D.bind();
		shader3D.setUniform("u_Model", gv.modelMatrixOCC);
		if (gv.cameraMode == centered)
		{
			axisOCC.draw(gv.modelMatrixOCC);
		}

		axis.draw(gv.modelMatrixOCC);





		/*shader3D.bind(); //This is for light, it is still there as far as shaders are concerned but it's not being rendered
		shader3D.setUniform("u_Model", gv.identityMatrix);
		shader3D.setUniform("u_Color", 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1);

		shader3D.setUniform("u_fragmentMode", 0);*/
		//light.draw();

		a.draw();
		
		/*a.draw();
		b.draw({1,0,0});*/
		c.draw();
		opaque();
	}
};