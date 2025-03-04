

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
#include "Axis.hpp"
#include "Ship.hpp"






//POLYHEDRA TAMBIÉN FUE MODIFICADO Y NECESITA ARREGLARSE ANTES DE DIVIDIR EL PROYECTO



int main(void)
{
	GLFWwindow* window = initialize();



	TimeStruct timeStruct;

	Shader shader3D("resources/shaders/shader3D.shader");
	Shader shader2D("resources/shaders/shader2D.shader");
	Shader shader2D_Instanced("resources/shaders/shader2D_Instanced.shader");
	Shader shaderText("resources/shaders/shaderText.shader");

	//main reason to add all the shaders there is to initialize the associated matrices in an encapsulated way
	Camera camera(window,shader3D, shader2D, shader2D_Instanced, shaderText);

	Settings settings(camera);








	//Falta poner texto estático, dinámico y multiples inputs en text to draw, texto en dpis, reserves
	Text text("resources/Glyphs/Helvetica/Helvetica.otf", 36);
	text.addText({ 10,950 }, timeStruct.fps, " fps");
	text.fillVertexBuffer();



	


	

	

	Lines2D arc;
	//vector<p2> a = createArc({ 300,300 }, 100, radians(270), radians(360));
	arc.addSet(createRoundedSquare(500, 0.1));
	//print(arc.positions);

	Lines2D_Instanced lines2D_Instanced;
	/*lines2D_Instanced.addSet({ {0,0},{100,0} });
	lines2D_Instanced.addInstances({{ {190, 100},  0, {10, 1} },{ {500, 100},  0, {1, 1} }});*/

	Lines3D globe;
	for (float i = -90; i <= 90; i += 10)
		globe.addSet(createLatitude(radians(i)));
	for (float i = -90; i <= 90; i += 10)
		globe.addSet(createLongitude(radians(i)));


	Axis axis(shader3D, camera); //I'm also adding light here and plan to add all general things here
	Ship ship(shader3D,camera);
	Ship shipOld(shader3D,camera);
	shipOld.shipTranslation = { 10,0,0 }; 
	shipOld.shipScale = 10;

	Graphics grafics;

	Map map(shader2D, camera);








	
	AllPointers allPointers(&camera, &map);
	glfwSetWindowUserPointer(window, &allPointers);
	glfwSetKeyCallback(window, keyboardEventCallback);
	glfwSetMouseButtonCallback(window, mouseEventCallback);





	//system("cls");
	while (!glfwWindowShouldClose(window))
	{
		getPos(window, mPos);
		if (isRunning)
		{
			timeStruct.update();	

			glClearColor(40 / 255.0f, 40 / 255.0f, 40 / 255.0f, 1.0f); //glClearColor(0.035f, 0.065f, 0.085f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



			axis.draw();
			ship.draw();
			//shipOld.draw();




			///////////////
			/////2d objects
			//transparent();
			//shader2D.bind();

			//glLineWidth(5);
			////arc.draw();
			//glLineWidth(1);

			////map.draw();


			////2d instanced
			//shader2D_Instanced.bind();
			//glLineWidth(1);
			//shader2D.setUniform("u_Color", 1, 1, 1, 1);
			////lines2D_Instanced.draw();
			////grafics.draw();


			/////////////
			//text
			transparent();
			shaderText.bind();
			text.sDraw();
			text.substituteText(0, { 10,950 }, round2d(timeStruct.fps), " fps"); // si no especificas position que no se mueva
			opaque();









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


