#pragma once

///////////////////////////////////////////////////////////////////////////////////
//RENDIMIENTO DE POLYHEDRA TIENE PRIORIDAD
//UNA VEZ TERMINADA LA OPTIMIZACIÓN JUNTAR COSAS AFINES

struct Ship {

	Shader& shader3D;
	Camera& camera;

	float angle = 0;

	Polyhedra stl;
	Polyhedra casco1, casco2, tapa1, tapa2, caja, timon, helice, motor, mecha, arbots, foilPopa, foilEstribor, foildBabor, aux, solar1, solar2;

	p3 shipTranslation = { 0,0,0 };
	//rotation for the full ship?
	float rudderAngle;

	Ship(Shader& shader3D_, Camera& camera_) :shader3D(shader3D_), camera(camera_) {

		readSimplePolyhedra(casco1, "casco1.bin");
		readSimplePolyhedra(casco2, "casco2.bin");


		/*readSTL(stl, "Tapa 1.STL");
		writeSimplePolyhedra(stl, "tapa1.bin");*/
		readSimplePolyhedra(tapa1, "tapa1.bin");

		/*readSTL(stl, "Tapa 2.STL");
		writeSimplePolyhedra(stl, "tapa2.bin");*/
		readSimplePolyhedra(tapa2, "tapa2.bin");

		/*readSTL(stl, "Caja.STL");
		writeSimplePolyhedra(stl, "caja.bin");*/
		readSimplePolyhedra(caja, "caja.bin");

		/*readSTL(stl, "Timon.STL");
		writeSimplePolyhedra(stl, "timonn.bin");*/
		readSimplePolyhedra(timon, "timonn.bin");

		/*readSTL(stl, "Helice.STL");
		writeSimplePolyhedra(stl, "helice.bin");*/
		readSimplePolyhedra(helice, "helice.bin");

		/*readSTL(stl, "Motor.STL");
		writeSimplePolyhedra(stl, "motor.bin");*/
		readSimplePolyhedra(motor, "motor.bin");

		/*readSTL(stl, "mecha.STL");
		writeSimplePolyhedra(stl, "mecha.bin");*/
		readSimplePolyhedra(mecha, "mecha.bin");

		/*readSTL(stl, "arbots.STL");
		writeSimplePolyhedra(stl, "arbots.bin");*/
		readSimplePolyhedra(arbots, "arbots.bin");

		/*readSTL(stl, "foil Popa.STL");
		writeSimplePolyhedra(stl, "foilPopa.bin");*/
		readSimplePolyhedra(foilPopa, "foilPopa.bin");

		/*readSTL(stl, "foil Estribor.STL");
		writeSimplePolyhedra(stl, "foilEstribor.bin");*/
		readSimplePolyhedra(foilEstribor, "foilEstribor.bin");

		/*readSTL(stl, "foil Babor.STL");
		writeSimplePolyhedra(stl, "foildBabor.bin");*/
		readSimplePolyhedra(foildBabor, "foildBabor.bin");

		/*readSTL(stl, "aux1.STL");
		writeSimplePolyhedra(stl, "aux1.bin");*/
		readSimplePolyhedra(aux, "aux1.bin");

		/*readSTL(stl, "solar1.STL");
		writeSimplePolyhedra(stl, "solar1.bin");*/
		readSimplePolyhedra(solar1, "solar1.bin");

		/*readSTL(stl, "solar2.STL");
		writeSimplePolyhedra(stl, "solar2.bin");*/
		readSimplePolyhedra(solar2, "solar2.bin");

		//print(casco1.positions);
	}

	void draw() {
		//EL QUE LA MATRIZ NO SE ACTUALIZE SI NO CAMBIAN SUS VALORES
		camera.model3DMatrix = camera.create3DModelMatrix(shipTranslation);
		shader3D.setUniform("u_Model", camera.model3DMatrix);


		shader3D.setUniform("u_fragmentMode", 0); //illumination

		shader3D.setUniform("u_Color", 0.0f / 255.0f, 127.0f / 255.0f, 0.0f / 255.0f, 1.0f);
		casco1.draw();
		casco2.draw();

		shader3D.setUniform("u_Color", 137.0f / 255.0f, 18.0f / 255.0f, 18.0f / 255.0f, 1.0f);
		tapa1.draw();
		tapa2.draw();

		shader3D.setUniform("u_Color", 0.0f / 255.0f, 63.0f / 255.0f, 200.0f / 255.0f, 1.0f);
		caja.draw();

		shader3D.setUniform("u_Color", 113.0f / 255.0f, 10.0f / 255.0f, 87.0f / 255.0f, 1.0f);
		arbots.draw();

		shader3D.setUniform("u_Color", 113.0f / 255.0f, 10.0f / 255.0f, 87.0f / 255.0f, 1.0f);
		timon.draw();

		shader3D.setUniform("u_Color", 0.0f / 255.0f, 63.0f / 255.0f, 200.0f / 255.0f, 1.0f);
		motor.draw();


		shader3D.setUniform("u_Color", 255 / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 1.0f);
		aux.draw();
		mecha.draw();

		shader3D.setUniform("u_Color", 16.0f / 255.0f, 28.0f / 255.0f, 82.0f / 255.0f, 1.0f);
		foilPopa.draw();

		shader3D.setUniform("u_Color", 16.0f / 255.0f, 28.0f / 255.0f, 82.0f / 255.0f, 1.0f);
		foilEstribor.draw();

		shader3D.setUniform("u_Color", 16.0f / 255.0f, 28.0f / 255.0f, 82.0f / 255.0f, 1.0f);
		foildBabor.draw();

		shader3D.setUniform("u_Color", 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1);
		helice.draw();

		
		//


		transparent();
		shader3D.setUniform("u_Color", 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 0.5);
		solar1.draw();
		shader3D.setUniform("u_Color", 0 / 255.0f, 0 / 255.0f, 0 / 255.0f, 1);
		solar2.draw();
		opaque();




		camera.model3DMatrix = camera.identityMatrix;
		shader3D.setUniform("u_Model", camera.model3DMatrix);
	}
};