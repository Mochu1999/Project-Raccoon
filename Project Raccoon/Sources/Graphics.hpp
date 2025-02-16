#pragma once
#include "Common.hpp"
#include "Circles.hpp"
#include "Lines2D_Instanced.hpp"

//Vamos a hacer las lineas con instancing. Partir de una única línea y enviar a la gpu toda la información para crear todas las instances.
//Ni de coña esto es más eficiente para crear lineas que van a ser casi siempre estáticas, pero tampoco es ningún drama y así practicamos

struct Graphics {
	Lines2D_Instanced grid;

	Graphics() {
		grid.addSet({ {0,0},{1,0} });

		grid.addInstances({
			//horizontal
			{ {100, 100},  0, {500, 1} },
			{ {100, 200},  0, {500, 1} },
			{ {100, 300},  0, {500, 1} },
			{ {100, 400},  0, {500, 1} },
			{ {100, 500},  0, {500, 1} },

			//vertical
			{ {100, 100},  radians(90), {500, 1} },
			{ {200, 100},  radians(90), {500, 1} },
			{ {300, 100},  radians(90), {500, 1} },
			{ {400, 100},  radians(90), {500, 1} },
			{ {500, 100},  radians(90), {500, 1} },
			});
	}


	void draw() {
		grid.draw();
	}
};


vector<p2> createRoundedSquare(float length, float roundedPercentage, p2 startingPos = { 200,200 }) {
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