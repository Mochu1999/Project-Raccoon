#pragma once

#include "Spheres.hpp"

struct Axis {
	Shader& shader3D;
	Camera& camera;
	
	Lines3D xLine, yLine, zLine, xLine2, zLine2;
	Sphere light;

	Axis(Shader& shader3D_, Camera& camera_) :shader3D(shader3D_), camera(camera_), light(3)
	{
		xLine.addSet({ {-100,0,0},{100,0,0} });
		yLine.addSet({ {0,-100,0},{0,100,0} });
		zLine.addSet({ {0,0,-100},{0,0,100} });
		xLine2.addSet({ {0,0,0},{5,0,0} });
		zLine2.addSet({ {0,0,0},{0,0,5} });


		
		p3 lightPos = { 30,25,40 };
		light.addSet(lightPos);

		shader3D.bind();
		shader3D.setUniform("u_lightPos", lightPos);
	}
	void draw() {

		shader3D.setUniform("u_Model", camera.identityMatrix);

		glLineWidth(2); //this is deprecated and platform dependent
		shader3D.setUniform("u_fragmentMode", 1);


		shader3D.setUniform("u_Color", 1.0, 0.0, 0.0, 1.0);
		xLine.draw();
		shader3D.setUniform("u_Color", 0.0, 1.0, 0.0, 1.0);
		yLine.draw();
		shader3D.setUniform("u_Color", 0.0, 0.0, 1.0, 1.0);
		zLine.draw();

		glLineWidth(1);

		shader3D.setUniform("u_Color", 1, 1, 1, 1.0);
		light.draw();
	}
};