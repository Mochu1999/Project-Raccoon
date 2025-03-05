#pragma once
#include "Common.hpp"
#include "Circles.hpp"
#include "Lines2D_Instanced.hpp"
#include "Ship.hpp"
//Vamos a hacer las lineas con instancing. Partir de una única línea y enviar a la gpu toda la información para crear todas las instances.
//Ni de coña esto es más eficiente para crear lineas que van a ser casi siempre estáticas, pero tampoco es ningún drama y así practicamos

vector<p2> createRoundedSquare(float length, float roundedPercentage, p2 startingPos = { 0,0 });

struct Graphic {

	Shader& shader2D;
	Shader& shader2D_Instanced;
	Camera& camera;
	Ship& ship;

	Lines2D_Instanced grid;

	Lines2D arc;
	Lines2D data;
	
	p2 gridCorner = { 1000,500 };

	std::array<float, 16> graphicModel2DMatrix;

	Graphic(Shader& shader2D_, Shader& shader2D_Instanced_, Camera& camera_, Ship& ship_)
		: shader2D(shader2D_), shader2D_Instanced(shader2D_Instanced_), camera(camera_), ship(ship_)
	{
		grid.addSet({ {0,0},{1,0} });

		float length = 100;
		grid.addInstances({
			//horizontal
			{ {0, 0},  0, {length, 1} },/*
			{ {0, 20},  0, {length, 1} },
			{ {0, 40},  0, {length, 1} },
			{ {0, 60},  0, {length, 1} },
			{ {0, 80},  0, {length, 1} },
			{ {0, 100},  0, {length, 1} },*/


			//vertical
			{ {0, 0},  radians(90), {length, 1} },/*
			{ {20, 0},  radians(90), {length, 1} },
			{ {40, 0},  radians(90), {length, 1} },
			{ {60, 0},  radians(90), {length, 1} },
			{ {80, 0},  radians(90), {length, 1} },
			{ {100, 0},  radians(90), {length, 1} },*/
			});

		arc.addSet(createRoundedSquare(500, 0.1));

		
	}


	void draw() {

		graphicModel2DMatrix = camera.create2DModelMatrix(gridCorner, 0, { 4,2 });


		
		transparent();


		shader2D.bind();

		shader2D.setUniform("u_Color", 1, 1, 1, 1);

		glLineWidth(4);
		//arc.draw();
		glLineWidth(1);

		shader2D_Instanced.bind();

		shader2D_Instanced.setUniform("u_Model", graphicModel2DMatrix);

		shader2D.setUniform("u_Color", 1, 1, 1, 1);

		grid.draw();
	}

};





vector<p2> createRoundedSquare(float length, float roundedPercentage, p2 startingPos) {
	vector<p2> positions;
	//reserve

	//starting arbitrarily on the bottom left corner
	//p2 startingPos = { 200,200 };

	float radius = length * roundedPercentage;

	vector<p2> temp = createArc(startingPos + radius, radius, radians(180), radians(270));
	positions.insert(positions.end(), temp.begin(), temp.end());

	temp = createArc({ startingPos.x + length - radius,startingPos.y + radius }, radius, radians(270), 0);
	positions.insert(positions.end(), temp.begin(), temp.end());

	temp = createArc(startingPos + (length - radius), radius, 0, radians(90));
	positions.insert(positions.end(), temp.begin(), temp.end());

	temp = createArc({ startingPos.x + radius,startingPos.y + length - radius }, radius, radians(90), radians(180));
	positions.insert(positions.end(), temp.begin(), temp.end());

	positions.emplace_back(positions.front());

	return positions;
}