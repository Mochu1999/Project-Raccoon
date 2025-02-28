#pragma once

struct Axis {
	Shader& shader3D;
	
	Lines3D xLine, yLine, zLine, xLine2, zLine2;
	

	Axis(Shader& shader3D_):shader3D(shader3D_)
	{
		xLine.addSet({ {-100,0,0},{100,0,0} });
		yLine.addSet({ {0,-100,0},{0,100,0} });
		zLine.addSet({ {0,0,-100},{0,0,100} });
		xLine2.addSet({ {0,0,0},{5,0,0} });
		zLine2.addSet({ {0,0,0},{0,0,5} });
	}
	void draw() {
		glLineWidth(2); //this is deprecated and platform dependent
		shader3D.setUniform("u_fragmentMode", 1);


		shader3D.setUniform("u_Color", 1.0, 0.0, 0.0, 1.0);
		xLine.draw();
		shader3D.setUniform("u_Color", 0.0, 1.0, 0.0, 1.0);
		yLine.draw();
		shader3D.setUniform("u_Color", 0.0, 0.0, 1.0, 1.0);
		zLine.draw();

		glLineWidth(1);
	}
};