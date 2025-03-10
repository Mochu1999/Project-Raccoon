#pragma once
#include "Common.hpp"
#include "Circles.hpp"
#include "Lines2D_Instanced.hpp"
#include "Ship.hpp"
#include "Text.hpp"
//Vamos a hacer las lineas con instancing. Partir de una única línea y enviar a la gpu toda la información para crear todas las instances.
//Ni de coña esto es más eficiente para crear lineas que van a ser casi siempre estáticas, pero tampoco es ningún drama y así practicamos

vector<p2> createRoundedSquare(float length, float roundedPercentage, p2 startingPos = { 0,0 });

struct Graphic {

	Shader& shader2D;
	Shader& shader2D_Instanced;
	Shader& shaderText;

	TimeStruct& tm;

	Camera& camera;
	Ship& ship;

	Lines2D_Instanced gridStatic;
	Lines2D_Instanced gridDynamic;

	Lines2D arc;
	Lines2D data;

	Text text;
	vector<Line> lines;

	std::array<float, 16> graphicModel2DMatrix;



	float counter = 0;
	p2 gridCorner = { 1000,500 }, dataCorner;
	float minDataX = 0, maxDataX = 0, minDataY = 0, maxDataY = 0;
	float scaleY = 1;
	float gridWidth = 400, gridHeight = 200;

	Graphic(Shader& shader2D_, Shader& shader2D_Instanced_, Shader& shaderText_, Camera& camera_, Ship& ship_, TimeStruct& tm_)
		: shader2D(shader2D_), shader2D_Instanced(shader2D_Instanced_), shaderText(shaderText_), tm(tm_)
		, camera(camera_), ship(ship_)
		, text("resources/Glyphs/Helvetica/Helvetica.otf", 16)
	{
		gridStatic.addSet({ {0,0},{1,0} });
		gridDynamic.addSet({ {0,0},{1,0} });


		gridStatic.addInstances({ //why do I have the scale as a feature if I am using the model matrix for it
			//horizontal
			{ {0, 0},  0, {1, 1} },/*
			{ {0, 20},  0, {length, 1} },
			{ {0, 40},  0, {length, 1} },
			{ {0, 60},  0, {length, 1} },
			{ {0, 80},  0, {length, 1} },
			{ {0, 100},  0, {length, 1} },*/


			//vertical
			{ {0, 0},  radians(90), {1, 1} },/*
			{ {20, 0},  radians(90), {length, 1} },
			{ {40, 0},  radians(90), {length, 1} },
			{ {60, 0},  radians(90), {length, 1} },
			{ {80, 0},  radians(90), {length, 1} },
			{ {100, 0},  radians(90), {length, 1} },*/
			});

		arc.addSet(createRoundedSquare(100, 0.2));

		
		//text.add({{gridCorner, 0," ",counter} });
		//text.addText(gridCorner, 0," ",counter);
		text.fillVertexBuffer();

		update();
	}

	void updateText() {

		
		lines = {
				{{gridCorner.x - 30,gridCorner.y - minDataY * scaleY}, 0},
				{{gridCorner.x - 30,gridCorner.y + (maxDataY - minDataY) * scaleY}, round1d(maxDataY)},
				{{gridCorner.x - 30,gridCorner.y }, round1d(minDataY)},
		};
		text.addDynamicText(lines);
	}

	void update() {
		pushData(c());

		dataCorner = gridCorner - p2{ maxDataX,minDataY * scaleY };

		updateText();
		

	}

	void draw() {
		if (tm.bUpdatePlot())update();

		




			transparent();


			shader2D.bind();

			shader2D.setUniform("u_Color", 40.0f / 255.0f, 239.9f / 255.0f, 239.0f / 255.0f, 1);


			glLineWidth(3);
			graphicModel2DMatrix = camera.create2DModelMatrix({ gridCorner.x - 50,gridCorner.y - 50 }, 0, { 5,3 });
			shader2D_Instanced.setUniform("u_Model", graphicModel2DMatrix);
			arc.draw();
			glLineWidth(1);


			graphicModel2DMatrix = camera.create2DModelMatrix(dataCorner, 0, { 1,scaleY });
			shader2D_Instanced.setUniform("u_Model", graphicModel2DMatrix);
			glEnable(GL_SCISSOR_TEST); //You'll need to try deque method just for potencial memory leaks
			glScissor(gridCorner.x, 0, 400, windowWidth);
			shader2D.setUniform("u_Color", 1, 1, 1, 1);
			data.draw();
			glDisable(GL_SCISSOR_TEST);









			shader2D_Instanced.bind();

			graphicModel2DMatrix = camera.create2DModelMatrix(gridCorner, 0, { gridWidth,gridHeight });
			shader2D_Instanced.setUniform("u_Model", graphicModel2DMatrix);

			shader2D.setUniform("u_Color", 1, 1, 1, 1);

			gridStatic.draw();

			shader2D.setUniform("u_Color", 0.5, 0.5, 0.5, 0.5);
			gridDynamic.addInstances({ { {0, -minDataY * scaleY / gridHeight},  0, {1, 1} } });
			gridDynamic.draw();





			shaderText.bind();
			//Este no va dividido por gridHeight, entiendo que por la carencia de model en Text, pero no termino de conectar el porqué
			//No sé como hacer para que round de siempre los mismos decimales
			//Haría una function que saque los 3 puntos, solo dibuje si hay suficiente separación e intermedios
			//linea al extremo superior
			
			text.draw();
		
	}

	int counter2 = 0;
	float c() {
		counter2 += 3;
		return (1 + counter2 / 10) * cos(radians(counter2));
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
			data.indices.push_back(size - 2);
			data.indices.push_back(size - 1);
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