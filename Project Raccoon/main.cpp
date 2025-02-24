

#include "Common.hpp"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"

#include "Lines3D.hpp"
#include "Lines2D.hpp"
#include "Lines2D_Instanced.hpp"

#include "Circles.hpp"

#include "Fourier.hpp"

#include "Camera.hpp"

#include "auxiliary_elements.hpp"

#include "BinariesManager.hpp"
#include "KeyMouseImputs.hpp"

#include "Polygons.hpp"
#include "Polygons2D.hpp"

#include "WettedSurface.hpp"

#include "AuxSquare.hpp"

#include "Spheres.hpp"

#include "Delaunay2D.hpp"
#include "Arrows.hpp"
#include "Pyramid.hpp"
#include "Model.hpp"
#include "Settings.hpp"

#include "Polyhedra.hpp"

#include "Text.hpp"

#include "auxiliary_elements.hpp"

#include "Graphics.hpp"

#include "Globe.hpp"
#include "Map.hpp"


using namespace std::chrono;

// to not render what is not visible to the camera:
//glEnable(GL_CULL_FACE);       // Enable face culling
//glCullFace(GL_BACK);          // Cull back faces
//glFrontFace(GL_CCW);          // Counter-clockwise winding is front-facing

//instance rendering for the future






int main(void)
{
	GLFWwindow* window = initialize();

	/*glDebugMessageCallback(MessageCallback, nullptr);
	glEnable(GL_DEBUG_OUTPUT);*/

	//BinariesManager binariesManager;





	/*Polyhedra stl, bin;

	TimeCounter tc1;
	readSTL(stl.positions, stl.normals, "Eiffel.STL");
	tc1.endCounter();

	stl.simpleIndices();


	TimeCounter tc2;
	readSimplePolyhedra(bin.positions, bin.normals, bin.indices, "Eiffel.bin");
	tc2.endCounter();

	print( tc1.endTime/ tc2.endTime);*/




	TimeStruct timeStruct;

	Shader shader3D("resources/shaders/shader3D.shader");
	Shader shader2D("resources/shaders/shader2D.shader");
	Shader shader2D_Instanced("resources/shaders/shader2D_Instanced.shader");
	Shader shaderText("resources/shaders/shaderText.shader");


	Camera camera(window);
	camera.updateCamera();

	Settings settings(camera);




	Sphere light(3);
	p3 lightPos = { 30,25,40 };
	light.addSet(lightPos);






	//Falta poner texto estático, dinámico y multiples inputs en text to draw, texto en dpis, reserves
	Text text("resources/Glyphs/Helvetica/Helvetica.otf", 36);
	text.addText({ 10,950 }, timeStruct.fps, " fps");
	/*text.addText("abcp 100,200.521", { 100, 100 });
	text.substituteText(1, "qwerqwetqwrtertqerggsdfggdfhslolE", { 100,100 });*/
	text.fillVertexBuffer();

	Lines3D xLine2;
	xLine2.addSet({ {0,0,0},{5,0,0} });
	Lines3D xLine;
	xLine.addSet({ {-100,0,0},{100,0,0} });
	Lines3D yLine;
	yLine.addSet({ {0,-100,0},{0,100,0} });
	Lines3D zLine;
	zLine.addSet({ {0,0,-100},{0,0,100} });
	Lines3D zLine2;
	zLine2.addSet({ {0,0,0},{0,0,5} });

	Polygons2D polygon2D; //Faltan reserves
	polygon2D.addSet({ {200,200 },{400,200},{400,400},{200,400},{200,200} });
	polygon2D.addSet({ {600,600},{800,600},{800,800},{600,800},{600,600} });


	Lines2D lines2d;
	//lines2d.addSet({ {36,64},{960,64},{960,420},{36,420},{36,64 } });
	lines2d.addSet({ {200,200 },{400,200},{400,400},{200,400},{200,200} });


	Circles circle(20, 4);
	circle.addSet({ {210,210},{300,500} });

	Polyhedra stl, timon, modelo;
	readSimplePolyhedra(timon.positions, timon.normals, timon.indices, "timon.bin");
	readSimplePolyhedra(modelo.positions, modelo.normals, modelo.indices, "modelo.bin");

	Lines2D arc;
	//vector<p2> a = createArc({ 300,300 }, 100, radians(270), radians(360));
	arc.addSet(createRoundedSquare(500, 0.1));
	//print(arc.positions);

	Lines2D_Instanced lines2D_Instanced;
	/*lines2D_Instanced.addSet({ {0,0},{100,0} });
	lines2D_Instanced.addInstances({
	{ {190, 100},  0, {10, 1} },
	{ {500, 100},  0, {1, 1} }
		});*/

	Lines3D globe;
	for (float i = -90; i <= 90; i += 10)
		globe.addSet(createLatitude(radians(i)));
	for (float i = -90; i <= 90; i += 10)
		globe.addSet(createLongitude(radians(i)));




	Graphics grafics;




	//Hola buenos días, objetivo encapsular en map (debería llamarse Autopilot?) todo esto. Para ello había pensado en primero
	//  encapsular todas las locations en una struct llamada locations y pasarle referencia a map para actualizar model color, shaders?.
	//  Va a ser un tema. Seguidamente hay que crear un background, un texto que siga el cursor y para mañana sería ideal dejar a que
	//  elijas o zoom o prototipo de ruta entre barcelona e ibiza con geodésicas.
	//  Muy buena suerte, tú puedes fácil con ello si te pones. Fuerzate un poquito al principio, rodará y acabarás féliz

	vector<vector<p2>> mapVectorOfVectors;
	Lines2D mercator;
	readVectorOfVectorsAscii(mapVectorOfVectors);

	for (auto& p : mapVectorOfVectors)
	{
		vector<p2>interm = lonLatTo2D(p);
		mercator.addSet(interm);
		//mercator.addSet(p);
		//print(interm);

	}



	//The region in 
	p2 point0 = mercator.positions[0];
	print(mercator.positions[0]);

	//the ratio width/height of the map square is:
	float totalX = mercator.positions[1].x - mercator.positions[0].x;
	float totalY = mercator.positions[2].y - mercator.positions[1].y;
	float naturalRatio = totalX / totalY;
	print(totalX); //4.22809e+06
	print(totalY); //3.52124e+06
	print(naturalRatio); //1.20074

	//first the arbitrary scaling, which is going to be that x is 1000 pixels long
	float scalingFactor = 1 / totalX * 1000;
	p2 mapCorner = { 200,100 }; //the coordinates of the bottom left coordinate of the map that will be applied after centering
	p2 translationFactor = { -point0.x * scalingFactor, -point0.y * scalingFactor }; //centering
	translationFactor += mapCorner;







	//////////////
	//Locations. They don't need to be binded to a shader in the creation but they do need to be binded when assigning the data
	///Look for uniform buffer objects. Used to send a lot of uniforms into a program more efficiently. I think it let's you share DATA BETWEEN SHADERS

	//3D
	int locationPerspective = glGetUniformLocation(shader3D.ID, "u_Perspective");
	int locationModel = glGetUniformLocation(shader3D.ID, "u_Model");
	int locationView = glGetUniformLocation(shader3D.ID, "u_View");
	int locationFragment = glGetUniformLocation(shader3D.ID, "u_fragmentMode");
	int locationCamPos = glGetUniformLocation(shader3D.ID, "u_CamPos");
	int locationColor3D = glGetUniformLocation(shader3D.ID, "u_Color");
	int locationLightPos = glGetUniformLocation(shader3D.ID, "u_lightPos");

	//2D
	int locationOrtho = glGetUniformLocation(shader2D.ID, "u_OrthoProjection");
	int locationModel2D = glGetUniformLocation(shader2D.ID, "u_Model2D");
	int locationColor2D = glGetUniformLocation(shader2D.ID, "u_Color");

	//2D_Instanced
	int locationOrtho_Instanced = glGetUniformLocation(shader2D_Instanced.ID, "u_OrthoProjection");
	int locationColor2D_Instanced = glGetUniformLocation(shader2D_Instanced.ID, "u_Color");

	//Text
	int locationOrthoText = glGetUniformLocation(shaderText.ID, "u_OrthoProjection");


	//////////////
	//INITIALAZING
	//3D
	shader3D.bind();
	glUniform3f(locationLightPos, lightPos.x, lightPos.y, lightPos.z);
	glUniformMatrix4fv(locationPerspective, 1, GL_FALSE, camera.perspectiveMatrix.data());
	glUniformMatrix4fv(locationModel, 1, GL_FALSE, camera.modelMatrix.data());

	//2D
	shader2D.bind();
	glUniformMatrix4fv(locationOrtho, 1, GL_FALSE, camera.orthoMatrix.data());
	camera.model2DMatrix = camera.createModel2DMatrix(translationFactor, 0, scalingFactor);
	glUniformMatrix4fv(locationModel2D, 1, GL_FALSE, camera.model2DMatrix.data());


	//2D_Instanced
	shader2D_Instanced.bind();
	glUniformMatrix4fv(locationOrtho_Instanced, 1, GL_FALSE, camera.orthoMatrix.data());

	//Text
	shaderText.bind();
	glUniformMatrix4fv(locationOrthoText, 1, GL_FALSE, camera.orthoMatrix.data());


	float angle = 0;
	AllPointers allPointers(&camera, &mercator);
	glfwSetWindowUserPointer(window, &allPointers);
	//getPos(window);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetKeyCallback(window, keyboardEventCallback);





	//system("cls");
	while (!glfwWindowShouldClose(window))
	{
		if (isRunning)
		{
			timeStruct.update();
			text.substituteText(0, { 10,950 }, round2d(timeStruct.fps), " fps"); // si no especificas position que no se mueva

			/////////////
			//3D
			shader3D.bind();
			//opaque objects first
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_TRUE);
			glDisable(GL_BLEND);

			glClearColor(0.035f, 0.065f, 0.085f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			glLineWidth(2); //this is deprecated and platform dependent
			glUniform1i(locationFragment, 1);
			//axis
			glUniform4f(locationColor3D, 1.0, 0.0, 0.0, 1.0);
			xLine.draw();
			glUniform4f(locationColor3D, 0.0, 1.0, 0.0, 1.0);
			yLine.draw();
			glUniform4f(locationColor3D, 0.0, 0.0, 1.0, 1.0);
			zLine.draw();


			glUniform4f(locationColor3D, 1, 1, 1, 1.0);
			light.draw();
			glLineWidth(1);
			camera.modelMatrix = camera.createModelMatrix({ 0,0,0 }, 0, { 0,0,0 }, 10);
			glUniformMatrix4fv(locationModel, 1, GL_FALSE, camera.modelMatrix.data());

			globe.draw();
			camera.modelMatrix = camera.identityMatrix;
			glUniformMatrix4fv(locationModel, 1, GL_FALSE, camera.modelMatrix.data());
			//gestionar location desde camera, más opciones de modelMatrix, una solo para escalar, solo rotar o solo transladar
			//  y combinaciones


			//model
			glUniform1i(locationFragment, 0);

			camera.modelMatrix = camera.createModelMatrix({ 40,0,0 }, angle, { 1,0,0 }, 1);
			//angle++;
			glUniformMatrix4fv(locationModel, 1, GL_FALSE, camera.modelMatrix.data());
			glUniform4f(locationColor3D, 255.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f, 1);
			timon.draw();
			glUniform4f(locationColor3D, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1);
			modelo.draw();
			camera.modelMatrix = camera.identityMatrix;
			glUniformMatrix4fv(locationModel, 1, GL_FALSE, camera.modelMatrix.data());




			////transparent objects
			//glEnable(GL_BLEND);
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			//glDepthMask(GL_FALSE);
			//glUniform1i(locationFragment, 0);
			//glUniform4f(locationColor3D, 40.0f / 255.0f, 239.9f / 255.0f, 239.0f / 255.0f, 0.6);




			/////////////
			///2d objects
			shader2D.bind();

			glLineWidth(5);
			glUniform4f(locationColor2D, 40.0f / 255.0f, 239.9f / 255.0f, 239.0f / 255.0f, 0.6);
			//arc.draw();
			glLineWidth(1);
			mercator.draw();

			shader2D_Instanced.bind();
			glLineWidth(1);
			glUniform4f(locationColor2D_Instanced, 1, 1, 1, 1);
			//lines2D_Instanced.draw();
			//grafics.draw();





			/////////////
			//text
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			shaderText.bind();
			text.sDraw();







			shader3D.bind();



			keyboardRealTimePolls(window, camera);
			camera.updateCamera();
			glUniform3f(locationCamPos, camera.cameraPos.x, camera.cameraPos.y, camera.cameraPos.z);
			glUniformMatrix4fv(locationView, 1, GL_FALSE, camera.viewMatrix.data());






			/*print(camera.forward);
			print(camera.cameraPos);*/

			//break;

		}
		//isRunning = false;
		

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	settings.write();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


