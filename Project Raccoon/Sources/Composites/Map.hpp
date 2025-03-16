#pragma once

//Pero t�o, por que no hay que incluir nada?

#include "Model.hpp"
#include "Circles.hpp"

//CAMERA EST� SOLO PARA LAS MODEL MATRICES, PLANTEATE SI TE INTERESA TENER una referencia de camera, porque bien podr�a solo tener
// una matriz aqu� local y hacer la funci�n que la crea global
//Ver que est� en uso, que no, reserves y comentarios
struct Map {
	Shader& shader2D;
	Camera& camera;
	Lines2D mercator;
	Polygons2D background;
	Polygons2D boxPositions; //ESTABLECER QUE ES DIN�MICO
	
	Circles circle;

	std::array<float, 16> mapModel2DMatrix;

	p2 point0;
	float totalX, totalY, naturalRatio, scalingFactor, scalingLocal = 1000 * 6;
	p2 mapCorner, translationTotal, translationFrame, translationLocal;
	vector<p2> frame;
	vector<float> frameLimits; //[0] x left, [1] x right, [2] y bottom, [3] y up
	bool isInsideFrame = 0;
	bool show = 0;



	Map(Shader& shader2D_, Camera& camera_) :shader2D(shader2D_), camera(camera_), circle(100) {


		vector<vector<p2>> mapVectorOfVectors;
		readVectorOfVectorsAscii(mapVectorOfVectors);

		for (auto& p : mapVectorOfVectors)
		{
			vector<p2>interm = lonLatTo2D(p);
			mercator.addSet(interm);

		}
		frame = lonLatTo2D(mapVectorOfVectors[0]);
		background.addSet(frame);

		//bottom left corner
		point0 = mercator.positions[0];


		//the ratio width/height of the map square is:
		totalX = mercator.positions[1].x - mercator.positions[0].x;
		totalY = mercator.positions[2].y - mercator.positions[1].y;
		naturalRatio = totalX / totalY;
		//print(totalX); //4.22809e+06
		//print(totalY); //3.52124e+06
		//print(naturalRatio); //1.20074

		circle.addSet({ {1000,500} });

		update();
	}

	void update() {

		//x is 1000 pixels long

		scalingFactor = 1 / totalX * scalingLocal; //scalingLocal is how many pixels of width the rendered map has
		mapCorner = { windowWidth/2-scalingLocal/2,windowHeight/2- scalingLocal / 2 }; //the coordinates of the bottom left coordinate of the map that will be applied after centering
		translationFrame = { -point0.x * scalingFactor, -point0.y * scalingFactor }; //moves the left corner to {0,0}
		translationFrame += mapCorner;

		for (auto& p : frame)
		{

			p *= scalingFactor;
			p += translationFrame;
		}
		frameLimits.push_back(frame[0].x);
		frameLimits.push_back(frame[1].x);
		frameLimits.push_back(frame[0].y);
		frameLimits.push_back(frame[2].y);
		//print(frameLimits);
		//se usa frame para algo? .clear?

		//corner in top left, moved with model matrix
		boxPositions.addSet({ {0,100}, {0,0},{100,0},{100,100},{0,100} });

		translationTotal = translationFrame + translationLocal;


	}

	void draw() {

		if (mPos.x > frameLimits[0] && mPos.x < frameLimits[1] && mPos.y > frameLimits[2] && mPos.y < frameLimits[3])
		{
			isInsideFrame = 1;
		}
		else
		{
			isInsideFrame = 0;

		}
		shader2D.bind();
		transparent();


		mapModel2DMatrix = camera.create2DModelMatrix(translationTotal, 0, scalingFactor);
		shader2D.setUniform("u_Model2D", mapModel2DMatrix);

		shader2D.setUniform("u_Color", 0.035f, 0.065f, 0.085f, 1.0f);
		background.draw();


		shader2D.setUniform("u_Color", 40.0f / 255.0f, 239.9f / 255.0f, 239.0f / 255.0f, 1);
		mercator.draw();

		if (isInsideFrame && show)
		{
			mapModel2DMatrix = camera.create2DModelMatrix(mPos, 0, 1);
			shader2D.setUniform("u_Model2D", mapModel2DMatrix);

			shader2D.setUniform("u_Color", 1, 1, 1, 1);
			boxPositions.draw();
		}
		shader2D.setUniform("u_Model2D", camera.identityMatrix);

		shader2D.setUniform("u_Color", 1, 0, 0, 1);
		circle.drawInterior();
	}
};