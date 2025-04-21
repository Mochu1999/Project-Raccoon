#pragma once

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