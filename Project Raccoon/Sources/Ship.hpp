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

	p3 shipHeave = { 0,0.5,0 }; float shipHeaveIncrease = 0.001;
	//rotation for the full ship?
	float rudderAngle = 0; float rudderIncrease = 0.3;
	float shipScale = 1;//borrar
	float propellerAngle = 0, propellerIncrease = 0.3;
	float foilAftAngle = 0, foilAftIncrease = 0.3;
	float foilStarboardAngle = 0, foilStarboardIncrease = 0.3;
	float foilPortAngle = 0, foilPortIncrease = 0.3;
	

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
		//{0,-1.001,0},{0.195, -1.001,0}
		readSimplePolyhedra(motor, "motor.bin");
		readSimplePolyhedra(mecha, "mecha.bin");
		readSimplePolyhedra(tapa3, "tapa3.bin");

		
		readSimplePolyhedra(foilPopa, "foilAft.bin");
		//{-0.023, -1.090,0}

		//BORRAR LOS .BIN QUE HAS MODIFICADO?
		readSimplePolyhedra(foilEstribor, "foilStarboard.bin");
		//{1.640,-1.089, 0.35}

		/*readSimplePolyhedra(stl, "foildBabor.bin");
		for (auto& p : stl.positions)
		{
			p -= {1.639, -1.090, -0.35};
		}
		writeSimplePolyhedra(stl, "foilPort.bin");*/
		readSimplePolyhedra(foildBabor, "foilPort.bin");
		//{1.639,-1.090,-0.35}

		readSimplePolyhedra(solar1, "solar1.bin");
		readSimplePolyhedra(solar2, "solar2.bin");


	}

	void draw() {

		std::array<float, 16> rudderMatrix, propellerMatrix, foilAftMatrix, foilStarboardMatrix, foilPortMatrix, rp, rf;

		if (rudderAngle >= 45 || rudderAngle <= -45)
			rudderIncrease = -rudderIncrease;

		if (foilAftAngle >= 20 || foilAftAngle <= -20)
			foilAftIncrease = -foilAftIncrease;

		if (foilStarboardAngle >= 20 || foilStarboardAngle <= -20)
			foilStarboardIncrease = -foilStarboardIncrease;

		if (foilPortAngle >= 20 || foilPortAngle <= -20)
			foilPortIncrease = -foilPortIncrease;

		if (shipHeave.y >= 0.7 || shipHeave.y <= 0.3)
			shipHeaveIncrease = -shipHeaveIncrease;

		rudderAngle += rudderIncrease;
		propellerAngle += 23;
		foilAftAngle += foilAftIncrease;
		foilStarboardAngle += foilStarboardIncrease;
		foilPortAngle += foilPortIncrease;
		shipHeave.y += shipHeaveIncrease;

		//u_model is being always changed to shipModel3DMatrix, but shipModel3DMatrix is only being created if input is changed

		//SOBRAN UN COJON DE OPERACIONES Y LIMPIAR
		shipModel3DMatrix = camera.identityMatrix;
		camera.translate3DModelMatrix(shipModel3DMatrix, shipHeave);
		shader3D.setUniform("u_Model", shipModel3DMatrix); //borrar esta del inicio, solo necesitas la del final


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

		transparent();
		shader3D.setUniform("u_Color", 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 0.5f);
		solar1.draw();

		shader3D.setUniform("u_Color", 0 / 255.0f, 0 / 255.0f, 0 / 255.0f, 1.0f);
		solar2.draw();
		opaque();


		//Timon
		{
			shipModel3DMatrix = camera.identityMatrix;
			
			camera.rotate3DModelMatrix(rudderMatrix, rudderAngle, { 0,1,0 });
			camera.translate3DModelMatrix(rudderMatrix, shipHeave);
			shader3D.setUniform("u_Model", rudderMatrix);


			shader3D.setUniform("u_Color", 113.0f / 255.0f, 10.0f / 255.0f, 87.0f / 255.0f, 1.0f);
			timon.draw();

			shader3D.setUniform("u_Color", 0.0f / 255.0f, 63.0f / 255.0f, 200.0f / 255.0f, 1.0f);
			motor.draw();

			shader3D.setUniform("u_Color", 137.0f / 255.0f, 18.0f / 255.0f, 18.0f / 255.0f, 1.0f);
			tapa3.draw();

			camera.rotate3DModelMatrix(propellerMatrix, propellerAngle, { 1,0,0 });
			rp = multiplyMatrices(rudderMatrix,propellerMatrix);

			p3 propellerTranslation = { 0, -1.001,0 }; 
			propellerTranslation.x += 0.195 * cos(radians(rudderAngle));
			propellerTranslation.z -= 0.195*sin(radians(rudderAngle));
			camera.translate3DModelMatrix(rp, propellerTranslation+shipHeave);
			shader3D.setUniform("u_Model", rp);
			shader3D.setUniform("u_Color", 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1);
			helice.draw();

			camera.rotate3DModelMatrix(foilAftMatrix, foilAftAngle, { 0,0,1 });
			rf = multiplyMatrices(rudderMatrix, foilAftMatrix);
			p3 foilAftTranslation = { 0, -1.090,0 };
			foilAftTranslation.x -= 0.023 * cos(radians(rudderAngle));
			foilAftTranslation.z += 0.023 * sin(radians(rudderAngle));
			camera.translate3DModelMatrix(rf, foilAftTranslation + shipHeave);
			shader3D.setUniform("u_Model", rf);

			shader3D.setUniform("u_Color", 16.0f / 255.0f, 28.0f / 255.0f, 82.0f / 255.0f, 1.0f);
			foilPopa.draw();

			

			shader3D.setUniform("u_Model", camera.identityMatrix);

		}

		camera.rotate3DModelMatrix(foilStarboardMatrix, foilStarboardAngle, { 0,0,1 });
		p3 foilStarboardTranslation = { 1.640,-1.089, 0.35 };
		camera.translate3DModelMatrix(foilStarboardMatrix, foilStarboardTranslation + shipHeave);
		shader3D.setUniform("u_Model", foilStarboardMatrix);
		shader3D.setUniform("u_Color", 16.0f / 255.0f, 28.0f / 255.0f, 82.0f / 255.0f, 1.0f);
		foilEstribor.draw();

		camera.rotate3DModelMatrix(foilPortMatrix, foilPortAngle, { 0,0,1 });
		p3 foilPortTranslation = { 1.640,-1.089, -0.35 };
		camera.translate3DModelMatrix(foilPortMatrix, foilPortTranslation + shipHeave);
		shader3D.setUniform("u_Model", foilPortMatrix);
		shader3D.setUniform("u_Color", 16.0f / 255.0f, 28.0f / 255.0f, 82.0f / 255.0f, 1.0f);
		foildBabor.draw();


		shader3D.setUniform("u_Model", camera.identityMatrix);

		


	}
};