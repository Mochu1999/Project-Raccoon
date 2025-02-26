

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

	Map map(shader2D, camera);



	



	






	//3D
	shader3D.bind();
	shader3D.setUniform("u_lightPos", lightPos);
	shader3D.setUniform("u_Perspective", camera.perspectiveMatrix);
	//shader3D.setUniform("u_Model", camera.modelMatrix);

	//2D
	shader2D.bind();
	shader2D.setUniform("u_OrthoProjection", camera.orthoMatrix);
	

	//2D_Instanced
	shader2D_Instanced.bind();
	shader2D_Instanced.setUniform("u_OrthoProjection", camera.orthoMatrix);

	//Text
	shaderText.bind();
	shaderText.setUniform("u_OrthoProjection", camera.orthoMatrix);



	float angle = 0;
	AllPointers allPointers(&camera, &map.mercator);
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
			shader3D.setUniform("u_fragmentMode", 1);
			
			//axis
			shader3D.setUniform("u_Color", 1.0, 0.0, 0.0, 1.0);
			xLine.draw();
			shader3D.setUniform("u_Color", 0.0, 1.0, 0.0, 1.0);
			yLine.draw();
			shader3D.setUniform("u_Color", 0.0, 0.0, 1.0, 1.0);
			zLine.draw();

			shader3D.setUniform("u_Color", 1, 1, 1, 1.0);
			light.draw();
			glLineWidth(1);
			camera.modelMatrix = camera.createModelMatrix({ 0,0,0 }, 0, { 0,0,0 }, 10);
			shader3D.setUniform("u_Model", camera.modelMatrix);

			globe.draw();
			camera.modelMatrix = camera.identityMatrix;
			shader3D.setUniform("u_Model", camera.modelMatrix);
			//gestionar location desde camera, más opciones de modelMatrix, una solo para escalar, solo rotar o solo transladar
			//  y combinaciones


			//model
			shader3D.setUniform("u_fragmentMode", 0);

			camera.modelMatrix = camera.createModelMatrix({ 40,0,0 }, angle, { 1,0,0 }, 1);
			camera.modelMatrix = camera.createModelMatrix({ 0,0,0 }, 0, { 0,0,0 }, 10);
			//angle++;
			shader3D.setUniform("u_Color", 255.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f, 1);
			timon.draw();
			shader3D.setUniform("u_Color", 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1);
			modelo.draw();
			camera.modelMatrix = camera.identityMatrix;
			shader3D.setUniform("u_Model", camera.modelMatrix);




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
			//arc.draw();
			glLineWidth(1);
			
			map.draw();
			
			
			//2d instanced
			shader2D_Instanced.bind();
			glLineWidth(1);
			shader2D.setUniform("u_Color", 1, 1, 1, 1);
			//lines2D_Instanced.draw();
			//grafics.draw();




			/////////////
			//text
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			shaderText.bind();
			text.sDraw();










			keyboardRealTimePolls(window, camera);
			camera.updateCamera();

			shader3D.bind();
			shader3D.setUniform("u_CamPos", camera.cameraPos);
			shader3D.setUniform("u_View", camera.viewMatrix);

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


