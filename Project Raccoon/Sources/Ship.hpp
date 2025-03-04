#pragma once

///////////////////////////////////////////////////////////////////////////////////
//RENDIMIENTO DE POLYHEDRA TIENE PRIORIDAD
//UNA VEZ TERMINADA LA OPTIMIZACIÓN JUNTAR COSAS AFINES

struct Ship {

	Shader& shader3D;
	Camera& camera;

	std::array<float, 16> shipModel3DMatrix = camera.identityMatrix;


	Polyhedra stl;
	Polyhedra casco1, casco2, tapa1, tapa2, tapa3, caja, timon, helice, motor, mecha, arbots
		, foilPopa, foilEstribor, foildBabor, aux, solar1, solar2;

	p3 shipTranslation = { 0,0,0 };
	//rotation for the full ship?
	float rudderAngle = 0; float rudderIncrease = 0.3;
	float shipScale = 1;//borrar

	Ship(Shader& shader3D_, Camera& camera_) :shader3D(shader3D_), camera(camera_) {


		readSimplePolyhedra(casco1, "casco1.bin");
		readSimplePolyhedra(casco2, "casco2.bin");
		readSimplePolyhedra(tapa1, "tapa1.bin");
		readSimplePolyhedra(tapa2, "tapa2.bin");
		readSimplePolyhedra(caja, "caja.bin");
		readSimplePolyhedra(arbots, "arbots.bin");
		readSimplePolyhedra(aux, "aux0.bin");

		readSimplePolyhedra(timon, "timon.bin");
		readSimplePolyhedra(helice, "helice.bin");
		readSimplePolyhedra(motor, "motor.bin");
		readSimplePolyhedra(mecha, "mecha.bin");
		readSimplePolyhedra(tapa3, "tapa3.bin");

		readSimplePolyhedra(foilPopa, "foilPopa.bin");
		readSimplePolyhedra(foilEstribor, "foilEstribor.bin");
		readSimplePolyhedra(foildBabor, "foildBabor.bin");

		readSimplePolyhedra(solar1, "solar1.bin");
		readSimplePolyhedra(solar2, "solar2.bin");


	}

	void draw() {


		if (rudderAngle >= 20 || rudderAngle <= -20)
			rudderIncrease = -rudderIncrease;
		rudderAngle += rudderIncrease;

		//u_model is being always changed to shipModel3DMatrix, but shipModel3DMatrix is only being created if input is changed
		//camera.create3DModelMatrix(shipModel3DMatrix, shipTranslation, shipScale);


		shader3D.setUniform("u_Model", camera.identityMatrix);


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

		shader3D.setUniform("u_Color", 255 / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 1.0f);
		aux.draw();
		mecha.draw();

		//Timon
		{
			camera.rotate3DModelMatrix(shipModel3DMatrix, rudderAngle, { 0,1,0 });
			shader3D.setUniform("u_Model", shipModel3DMatrix);


			shader3D.setUniform("u_Color", 113.0f / 255.0f, 10.0f / 255.0f, 87.0f / 255.0f, 1.0f);
			timon.draw();

			shader3D.setUniform("u_Color", 0.0f / 255.0f, 63.0f / 255.0f, 200.0f / 255.0f, 1.0f);
			motor.draw();

			shader3D.setUniform("u_Color", 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1);
			helice.draw();

			shader3D.setUniform("u_Color", 16.0f / 255.0f, 28.0f / 255.0f, 82.0f / 255.0f, 1.0f);
			foilPopa.draw();

			shader3D.setUniform("u_Color", 137.0f / 255.0f, 18.0f / 255.0f, 18.0f / 255.0f, 1.0f);
			tapa3.draw();

			shader3D.setUniform("u_Model", camera.identityMatrix);

		}


		shader3D.setUniform("u_Color", 16.0f / 255.0f, 28.0f / 255.0f, 82.0f / 255.0f, 1.0f);
		foilEstribor.draw();

		shader3D.setUniform("u_Color", 16.0f / 255.0f, 28.0f / 255.0f, 82.0f / 255.0f, 1.0f);
		foildBabor.draw();




		//transparent();
		shader3D.setUniform("u_Color", 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 0.5f);
		solar1.draw();

		shader3D.setUniform("u_Color", 0 / 255.0f, 0 / 255.0f, 0 / 255.0f, 1.0f);
		solar2.draw();
		opaque();


	}
};