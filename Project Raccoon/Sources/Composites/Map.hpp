#pragma once

//pocketGrib

//Pero tío, por que no hay que incluir nada?

#include "Model.hpp"
#include "Circles.hpp"
#include "Text.hpp"

//Mirar el tema de los rumbos circulares

//CAMERA ESTÁ SOLO PARA LAS MODEL MATRICES, PLANTEATE SI TE INTERESA TENER una referencia de camera, porque bien podría solo tener
// una matriz aquí local y hacer la función que la crea global
	//Crear matrices locales permitirá no recrear la matriz cada vez a coste de mayor memoria y no necesitar camara en muchas structs

struct IconLight {

	Shader& shader2D;
	Camera& camera;


	Polyhedra interm;
	Polygons2D mainHull, sideHull1, sideHull2, aux1, aux2, aux3;
	Lines2D auxa, auxb;

	IconLight(Shader& shader2D_, Camera& camera_) :shader2D(shader2D_), camera(camera_)
	{

		readSTL(interm, "C:/dev/Project-Raccoon/Project Raccoon/Resources/stl models/mainHull.stl");
		polyhedraTo2D(interm, mainHull);

		readSTL(interm, "C:/dev/Project-Raccoon/Project Raccoon/Resources/stl models/sideHull1.stl");
		polyhedraTo2D(interm, sideHull1);

		readSTL(interm, "C:/dev/Project-Raccoon/Project Raccoon/Resources/stl models/sideHull2.stl");
		polyhedraTo2D(interm, sideHull2);

		readSTL(interm, "C:/dev/Project-Raccoon/Project Raccoon/Resources/stl models/aux1.stl");
		polyhedraTo2D(interm, aux1);

		readSTL(interm, "C:/dev/Project-Raccoon/Project Raccoon/Resources/stl models/aux2.stl");
		polyhedraTo2D(interm, aux2);

		readSTL(interm, "C:/dev/Project-Raccoon/Project Raccoon/Resources/stl models/aux3.stl");
		polyhedraTo2D(interm, aux3);


		auxa.addSet({ {-5,0},{5,0} });
		auxb.addSet({ {0,-5},{0,5} });

	}

	void draw() {

		shader2D.bind();
		/*std::array<float, 16> mapModel2DMatrix = camera.create2DModelMatrix({0,0}, 0, 50);
		shader2D.setUniform("u_Model", mapModel2DMatrix);*/

		shader2D.setUniform("u_Color", 1, 1, 0, 1);
		mainHull.draw();
		sideHull1.draw();
		sideHull2.draw();

		shader2D.setUniform("u_Color", 1, 1, 1, 1);
		aux1.draw();
		aux2.draw();
		aux3.draw();

		/*shader2D.setUniform("u_Color", 1, 0, 0, 1);
		glLineWidth(3);
		auxa.draw();
		auxb.draw();
		glLineWidth(1);*/
	}

};



//Ver que está en uso, que no, reserves y comentarios
struct Map {
	Shader& shader2D, shaderText;
	Camera& camera;
	GlobalVariables& gv;

	Lines2D mercator;
	Polygons2D background;
	Polygons2D boxPositions; //Where the text is after pressing S //ESTABLECER QUE ES DINÁMICO

	IconLight icon;

	Circles circle;


	std::array<float, 16> mapModel2DMatrix;

	p2 point0;
	float totalX, totalY, naturalRatio, scalingFactor;
	float totalPixels = 1000 * 6; //This makes totalX 6000 pixels long

	p2 mapCorner, translationTotal;
	vector<p2> frame;
	vector<float> frameLimits; //[0] x left, [1] x right, [2] y bottom, [3] y up
	bool show = 0;

	Text text1, text2;
	p2 shipCoordinates = { 2.128842,41.248926 };
	p2 finishPoint = { 1.25,39.05 };

	float totalDistance;
	Lines2D dataBoxOutline;
	Polygons2D dataBox;


	Lines2D courseLine;

	Map(Shader& shader2D_, Shader& shaderText_, Camera& camera_, GlobalVariables& gv_)
		:shader2D(shader2D_), shaderText(shaderText_),
		camera(camera_), circle(5000, 100), text1("resources/Glyphs/Helvetica/Helvetica.otf", 36), text2("resources/Glyphs/Helvetica/Helvetica.otf", 36)
		, gv(gv_), icon(shader2D, camera)
	{


		vector<vector<p2>> mapVectorOfVectors;
		readVectorOfVectorsAscii(mapVectorOfVectors);

		for (auto& p : mapVectorOfVectors)
		{
			vector<p2>interm = lonLatToMercator(p);
			mercator.addSet(interm);

		}
		frame = lonLatToMercator(mapVectorOfVectors[0]);
		background.addSet(frame);

		//bottom left corner in meters from earth's 0,0 
		point0 = mercator.positions[0];



		totalX = mercator.positions[1].x - mercator.positions[0].x;
		totalY = mercator.positions[2].y - mercator.positions[1].y;
		//print(totalX); //4.22809e+06
		//print(totalY); //3.52124e+06
		//print(totalX / totalY); //1.20074

		courseLine.addSet({ lonLatToMercator(shipCoordinates),lonLatToMercator(finishPoint) });

		circle.addSet(lonLatToMercator({ finishPoint }));

		dataBoxOutline.addSet(createRoundedSquare({ 0,0 }, { 500,500 }, 50));
		dataBox.addSet(createRoundedSquare({ 0,0 }, { 500,500 }, 50));


		update();

		//If the distance doesn't make sense dude to very small angles use Haversine formula

	}

	void update() {


		//Multiplying the map positions by scalingFactor in the model matrix will make all coordinates fall between 0 and totalPixels (6000 pixels)
		scalingFactor = 1 / totalX * totalPixels;

		//after scalingFactor, point0 falls in {-500.242,652.636}. We will sum that amount to put point0 in O
		translationTotal = { -point0.x * scalingFactor, -point0.y * scalingFactor };
		//we end with a centered left corner and a map that is totalPixels long


		//after having the map in the corner we move it to an arbitrary half of the scali
		mapCorner = { windowWidth / 2 - totalPixels / 2,windowHeight / 2 - totalPixels / 2 };
		translationTotal += mapCorner;

		/*print(translationTotal);
		print(scalingFactor);*/

		for (auto& p : frame)
		{
			p *= scalingFactor;
			p += translationTotal;
		}

		frameLimits.insert(frameLimits.end(), { frame[0].x, frame[1].x, frame[0].y , frame[2].y });
		//se usa frame para algo? .clear?


		boxPositions.addSet({ {0,0},{300,0},{300,100},{0,100},{0,0} });
	}

	void draw() {
		p2 a = (lonLatToMercator(shipCoordinates) - point0) * scalingFactor + mapCorner;
		p2 cursorVal = mercatorToLonLat((gv.mPos - translationTotal) / scalingFactor);
		totalDistance = distanceOnSphere(shipCoordinates, finishPoint);


		//shipCoordinates = cursorVal; //en movimiento el barco rota con un delay de un segundo, por qué?

		shader2D.bind();
		transparent();


		mapModel2DMatrix = camera.create2DModelMatrix(translationTotal, 0, scalingFactor);
		shader2D.setUniform("u_Model", mapModel2DMatrix);

		shader2D.setUniform("u_Color", 0.035f, 0.065f, 0.085f, 1.0f);
		background.draw();


		shader2D.setUniform("u_Color", 40.0f / 255.0f, 239.9f / 255.0f, 239.0f / 255.0f, 1);
		mercator.draw();



		//print(0.65346 * earthRadius /1000);


		//surprisingly it won't work without the lonlat. I don't know why. Point error? Projection?
		p2 vect = lonLatToMercator(finishPoint) - lonLatToMercator(shipCoordinates);
		float angle = degrees(atan2(vect.y, vect.x));



		courseLine.clear();
		courseLine.addSet({ lonLatToMercator(shipCoordinates),lonLatToMercator(finishPoint) });
		shader2D.setUniform("u_Color", 1, 0.3, 0, 0.5);
		glLineWidth(1);
		courseLine.draw();
		glLineWidth(1);



		shader2D.setUniform("u_Color", 1, 0, 0, 1);
		circle.drawInterior();


		//Icon can't use the map model matrix, it would need have its positions at 2e+06 for it
		mapModel2DMatrix = camera.create2DModelMatrix(a, angle, 50);
		shader2D.setUniform("u_Model", mapModel2DMatrix);
		icon.draw();





		if (show)
		{
			mapModel2DMatrix = camera.create2DModelMatrix(gv.mPos, 0, 1);
			shader2D.setUniform("u_Model", mapModel2DMatrix);

			shader2D.setUniform("u_Color", 1, 1, 1, 1);
			boxPositions.draw();
		}
		//shader2D.setUniform("u_Model", camera.identityMatrix);

		shaderText.bind();
		//static stuff
		text1.addDynamicText({
			{{ 100,750 }, finishPoint.x,", ", finishPoint.y},
			{{ 100,700 }, shipCoordinates.x,", ", shipCoordinates.y},
			{ { 100,650 }, totalDistance * 10e-3,"km"} });

		text1.draw();


		text2.addDynamicText({ {gv.mPos, round2d(cursorVal.x),", ", round2d(cursorVal.y)} });

		//text2.draw();

		shader2D.bind();
		shader2D.setUniform("u_Model", camera.identityMatrix);
		shader2D.setUniform("u_Color", 0.035f, 0.065f, 0.085f, 1.0f);
		dataBox.draw();
		shader2D.setUniform("u_Color", 40.0f / 255.0f, 239.9f / 255.0f, 239.0f / 255.0f, 1);

		glLineWidth(3);
		dataBoxOutline.draw();
		glLineWidth(1);


	}
};
