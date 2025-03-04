#pragma once

//Pero tío, por que no hay que incluir nada?

#include "Model.hpp"

//CAMERA ESTÁ SOLO PARA LAS MODEL MATRICES, PLANTEATE SI TE INTERESA TENER una referencia de camera, porque bien podría solo tener
// una matriz aquí local y hacer la función que la crea global
//Ver que está en uso, que no, reserves y comentarios
struct Map {
	Shader& shader2D;
	Camera& camera;
	Lines2D mercator;
	Polygons2D background;
	Polygons2D boxPositions; //ESTABLECER QUE ES DINÁMICO

	float totalX, totalY, naturalRatio, scalingFactor;
	p2 mapCorner, translationFactor;
	vector<p2> frame;
	vector<float> frameLimits; //[0] x left, [1] x right, [2] y bottom, [3] y up
	bool isInsideFrame =0;
	bool show = 0;

	Map(Shader& shader2D_, Camera& camera_) :shader2D(shader2D_), camera(camera_) {



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
		p2 point0 = mercator.positions[0];


		//the ratio width/height of the map square is:
		totalX = mercator.positions[1].x - mercator.positions[0].x;
		totalY = mercator.positions[2].y - mercator.positions[1].y;
		naturalRatio = totalX / totalY;
		//print(totalX); //4.22809e+06
		//print(totalY); //3.52124e+06
		//print(naturalRatio); //1.20074

		//first the arbitrary scaling, which is going to be that x is 1000 pixels long
		scalingFactor = 1 / totalX * 1000;
		mapCorner = { 200,100 }; //the coordinates of the bottom left coordinate of the map that will be applied after centering
		translationFactor = { -point0.x * scalingFactor, -point0.y * scalingFactor }; //centering
		translationFactor += mapCorner;

		for (auto& p : frame) 
		{

			p *= scalingFactor;
			p += translationFactor;
		}
		frameLimits.push_back(frame[0].x);
		frameLimits.push_back(frame[1].x);
		frameLimits.push_back(frame[0].y);
		frameLimits.push_back(frame[2].y);
		//print(frameLimits);
		//se usa frame para algo? .clear?

		//corner in top left, moved with model matrix
		boxPositions.addSet({ {0,100}, {0,0},{100,0},{100,100},{0,100} });
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

		camera.model2DMatrix = camera.create2DModelMatrix(translationFactor, 0, scalingFactor);
		shader2D.setUniform("u_Model2D", camera.model2DMatrix);

		shader2D.setUniform("u_Color", 0.035f, 0.065f, 0.085f, 1.0f);
		background.draw();


		shader2D.setUniform("u_Color", 40.0f / 255.0f, 239.9f / 255.0f, 239.0f / 255.0f, 1);

		mercator.draw();

		if (isInsideFrame && show)
		{
			camera.model2DMatrix = camera.create2DModelMatrix(mPos, 0, 1);
			shader2D.setUniform("u_Model2D", camera.model2DMatrix);

			shader2D.setUniform("u_Color", 1, 1, 1, 1);
			boxPositions.draw();
		}
		shader2D.setUniform("u_Model2D", camera.identityMatrix);
	}
};