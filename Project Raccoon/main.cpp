

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
#include "Overlay2D.hpp"





//POLYHEDRA TAMBI�N FUE MODIFICADO Y NECESITA ARREGLARSE ANTES DE DIVIDIR EL PROYECTO



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








	//Falta poner texto est�tico, din�mico y multiples inputs en text to draw, texto en dpis, reserves
	Text text("resources/Glyphs/Helvetica/Helvetica.otf", 36);
	text.addText({ 10,950 }, timeStruct.fps, " fps");
	text.fillVertexBuffer();



	


	Axis axis(shader3D, camera); //I'm also adding light here and plan to add all general things here
	Ship ship(shader3D,camera);

	Overlay2D overlay(shader2D,camera);
	Graphic graphic(shader2D,shader2D_Instanced,camera,ship);

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


			overlay.draw();
			graphic.draw();
			//map.draw();






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


