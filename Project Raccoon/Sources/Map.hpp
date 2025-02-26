#pragma once

//CAMERA ESTÁ SOLO PARA LAS MODEL MATRICES, PLANTEATE SI TE INTERESA TENER una referencia de camera, porque bien podría solo tener
// una matriz aquí local y hacer la función que la crea global

struct Map {
	Shader& shader2D;
	Camera& camera;
	Lines2D mercator;
	Polygons2D background;

	float totalX, totalY, naturalRatio, scalingFactor;
	p2 mapCorner, translationFactor;

	Map(Shader& shader2D_, Camera& camera_) :shader2D(shader2D_), camera(camera_) {

		background.addSet({ {0,0},{200,0},{200,200},{0,200},{0,0} });


		vector<vector<p2>> mapVectorOfVectors;
		readVectorOfVectorsAscii(mapVectorOfVectors);

		for (auto& p : mapVectorOfVectors)
		{
			vector<p2>interm = lonLatTo2D(p);
			mercator.addSet(interm);

		}


		//The region in 
		p2 point0 = mercator.positions[0];
		print(mercator.positions[0]);


		//the ratio width/height of the map square is:
		totalX = mercator.positions[1].x - mercator.positions[0].x;
		totalY = mercator.positions[2].y - mercator.positions[1].y;
		naturalRatio = totalX / totalY;
		print(totalX); //4.22809e+06
		print(totalY); //3.52124e+06
		print(naturalRatio); //1.20074

		//first the arbitrary scaling, which is going to be that x is 1000 pixels long
		scalingFactor = 1 / totalX * 1000;
		mapCorner = { 200,100 }; //the coordinates of the bottom left coordinate of the map that will be applied after centering
		translationFactor = { -point0.x * scalingFactor, -point0.y * scalingFactor }; //centering
		translationFactor += mapCorner;
	}

	void draw() {
		shader2D.setUniform("u_Color", 1, 1, 1, 1);
		background.draw();


		shader2D.setUniform("u_Color", 40.0f / 255.0f, 239.9f / 255.0f, 239.0f / 255.0f, 0.6);

		camera.model2DMatrix = camera.createModel2DMatrix(translationFactor, 0, scalingFactor);
		shader2D.setUniform("u_Model2D", camera.model2DMatrix);
		mercator.draw();
		shader2D.setUniform("u_Model2D", camera.createIdentityMatrix());
	}
};