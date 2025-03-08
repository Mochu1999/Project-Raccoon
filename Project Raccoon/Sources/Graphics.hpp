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

	Lines2D_Instanced gridStatic;
	Lines2D_Instanced gridDynamic;

	Lines2D arc;
	Lines2D data;
	float counter=0;

	p2 gridStaticCorner = { 1000,500 };

	std::array<float, 16> graphicModel2DMatrix;

	
	float minDataX = 0, maxDataX = 0, minDataY = 0, maxDataY = 0;
	float scaleY = 1;

	Graphic(Shader& shader2D_, Shader& shader2D_Instanced_, Camera& camera_, Ship& ship_)
		: shader2D(shader2D_), shader2D_Instanced(shader2D_Instanced_), camera(camera_), ship(ship_)
	{
		gridStatic.addSet({ {0,0},{1,0} });
		gridDynamic.addSet({ {0,0},{1,0} });

		float length = 100;
		gridStatic.addInstances({
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

		//vector<p2> positions = {{0, 0.3}, { 1,0.6 }, { 2,0.9 }, { 3,1.2 }, { 4,1.5 }, { 5,1.8 }, { 6,2.1 }, { 7,2.4 }, { 8,2.7 }, { 9,3 }, { 10,3.3 }, { 11,3.6 }, { 12,3.9 }, { 13,4.2 }, { 14,4.5 }, { 15,4.8 }, { 16,5.1 }, { 17,5.4 }, { 18,5.7 }, { 19,6 }, { 20,6.3 }, { 21,6.6 }, { 22,6.9 }, { 23,7.2 }, { 24,7.5 }, { 25,7.8 }, { 26,8.1 }, { 27,8.4 }, { 28,8.7 }, { 29,9 }, { 30,9.3 }, { 31,9.6 }, { 32,9.9 }, { 33,10.2 }, { 34,10.5 }, { 35,10.8 }, { 36,11.1 }, { 37,11.4 }, { 38,11.7 }, { 39,12 }, { 40,12.3 }, { 41,12.6 }, { 42,12.9 }, { 43,13.2 }, { 44,13.5 }, { 45,13.8 }, { 46,14.1 }, { 47,14.4 }, { 48,14.7 }, { 49,15 }, { 50,15.3 }, { 51,15.6 }, { 52,15.9 }, { 53,16.2 }, { 54,16.5 }, { 55,16.8 }, { 56,17.1 }, { 57,17.4 }, { 58,17.7 }, { 59,18 }, { 60,18.3 }, { 61,18.6 }, { 62,18.9 }, { 63,19.2 }, { 64,19.5 }, { 65,19.8 }, { 66,20.1 }, { 67,19.8 }, { 68,19.5 }, { 69,19.2 }, { 70,18.9 }, { 71,18.6 }, { 72,18.3 }, { 73,18 }, { 74,17.7 }, { 75,17.4 }, { 76,17.1 }, { 77,16.8 }, { 78,16.5 }, { 79,16.2 }, { 80,15.9 }, { 81,15.6 }, { 82,15.3 }, { 83,15 }, { 84,14.7 }, { 85,14.4 }, { 86,14.1 }, { 87,13.8 }, { 88,13.5 }, { 89,13.2 }, { 90,12.9 }, { 91,12.6 }, { 92,12.3 }, { 93,12 }, { 94,11.7 }, { 95,11.4 }, { 96,11.1 }, { 97,10.8 }, { 98,10.5 }, { 99,10.2 }, { 100,9.9 }, { 101,9.6 }, { 102,9.3 }, { 103,9 }, { 104,8.7 }, { 105,8.4 }};
		//data.positions = positions;
		////data.addSet(positions);
		//data.indices = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 23, 24, 24, 25, 25, 26, 26, 27, 27, 28, 28, 29, 29, 30, 30, 31, 31, 32, 32, 33, 33, 34, 34, 35, 35, 36, 36, 37, 37, 38, 38, 39, 39, 40, 40, 41, 41, 42, 42, 43, 43, 44, 44, 45, 45, 46, 46, 47, 47, 48, 48, 49, 49, 50, 50, 51, 51, 52, 52, 53, 53, 54, 54, 55, 55, 56, 56, 57, 57, 58, 58, 59, 59, 60, 60, 61, 61, 62, 62, 63, 63, 64, 64, 65, 65, 66, 66, 67, 67, 68, 68, 69, 69, 70, 70, 71, 71, 72, 72, 73, 73, 74, 74, 75, 75, 76, 76, 77, 77, 78, 78, 79, 79, 80, 80, 81, 81, 82, 82, 83, 83, 84, 84, 85, 85, 86, 86, 87, 87, 88, 88, 89, 89, 90, 90, 91, 91, 92, 92, 93, 93, 94, 94, 95, 95, 96, 96, 97, 97, 98, 98, 99, 99, 100, 100, 101, 101, 102, 102, 103, 103, 104, 104, 105, 105};
		////data.indices = { 69, 105};
		//data.isBufferUpdated = true;

		//print(data.positions);
		//print(data.indices);
		//print(data.positions.size());
	
	}



	void draw() {

		pushData(c());
		//print(c());
		/*print(data.positions);
		print(data.indices);*/
		{



			transparent();


			shader2D.bind();

			shader2D.setUniform("u_Color", 1, 1, 1, 1);

			glLineWidth(4);
			//arc.draw();
			glLineWidth(1);

			float mediumY = maxDataY - minDataY;
			p2 dataCorner = gridStaticCorner - p2{ maxDataX,minDataY*scaleY };
			graphicModel2DMatrix = camera.create2DModelMatrix(dataCorner, 0, {1,scaleY});
			shader2D_Instanced.setUniform("u_Model", graphicModel2DMatrix);
			data.draw();

			shader2D_Instanced.bind();

			graphicModel2DMatrix = camera.create2DModelMatrix(gridStaticCorner, 0, { 4,2 });
			shader2D_Instanced.setUniform("u_Model", graphicModel2DMatrix);

			shader2D.setUniform("u_Color", 1, 1, 1, 1);

			gridStatic.draw();
			gridDynamic.draw();
		}

		
	}

	int counter2=0;
	float c() {
		counter2 += 10;
		return (1+ counter2 / 10) * cos(radians(counter2));
	}

	void pushData(float f) {

		if (f < minDataY) minDataY = f;
		if (f > maxDataY) maxDataY = f;
		if (counter > 400) maxDataX++;
		if (maxDataY - minDataY > 200) scaleY = 200 / (maxDataY - minDataY);

		data.positions.push_back({ counter,f });
		counter++;

		createIndices();
		data.isBufferUpdated = true;
	}
	void createIndices() {
		size_t size = data.positions.size();
		if (size > 1)
		{
			data.indices.push_back(size-2);
			data.indices.push_back(size-1);
		}
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