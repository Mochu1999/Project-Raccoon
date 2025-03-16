

#include "Common.hpp"


#include "Shader.hpp"
#include "Time.hpp"

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
#include "ProgressBar.hpp"



float c = 1;
float graf1Val= 0;
float cactus(float& c) {
	c += 2.5;
	return (1 * c / 10) * cos(radians(c));
}



int main(void)
{
	GLFWwindow* window = initialize();



	TimeStruct tm;

	Shader shader3D("resources/shaders/shader3D.shader");
	Shader shader2D("resources/shaders/shader2D.shader");
	Shader shader2D_Instanced("resources/shaders/shader2D_Instanced.shader");
	Shader shaderText("resources/shaders/shaderText.shader");

	//main reason to add all the shaders there is to initialize the associated matrices in an encapsulated way
	Camera camera(window, shader3D, shader2D, shader2D_Instanced, shaderText);

	Settings settings(camera);








	Text text("resources/Glyphs/Helvetica/Helvetica.otf", 36);
	text.addText({ {{ 10,950 }, tm.fps, " fps"},{{10,1000},tm.currentTime, " s"} });


	Lines2D arc;
	arc.addSet({ {0,0}, { 100,0 },{100,100},{0,100},{0,0} });
	


	Axis axis(shader3D, camera); //I'm also adding light here and plan to add all general things here
	Ship ship(shader3D,camera);

	Overlay2D overlay(shader2D,camera);
	Graphic graphic(shader2D, shader2D_Instanced, shaderText, camera, ship, tm, "A*cos(x)", { 1400,100 }, graf1Val);
	Graphic graphic2(shader2D, shader2D_Instanced, shaderText, camera, ship, tm, "rudderAngle", { 1400,400 }, ship.rudderAngle);
	ProgressBar pb(shader2D, shader2D_Instanced, shaderText, camera, ship, tm, { 1400-50,700 });

	Map map(shader2D, camera);





	
	AllPointers allPointers(&camera, &map);
	glfwSetWindowUserPointer(window, &allPointers);
	glfwSetKeyCallback(window, keyboardEventCallback);
	glfwSetMouseButtonCallback(window, mouseEventCallback);



	int counter = 0;

	//system("cls");
	while (!glfwWindowShouldClose(window))
	{
		getPos(window, mPos);
		if (isRunning)
		{
			tm.update();	

			glClearColor(40 / 255.0f, 40 / 255.0f, 40 / 255.0f, 1.0f); //glClearColor(0.035f, 0.065f, 0.085f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



			axis.draw();
			ship.draw();


			overlay.draw();

			graf1Val = cactus(c);
			graphic.draw();
			graphic2.draw();
			pb.draw();

			//map.draw();



			/////////////
			//text
			transparent();
			shaderText.bind();
			text.draw();
			text.substituteText(0, { { 10,950 }, round2d(tm.fps), " fps" });
			text.substituteText(1, round1d(tm.currentTime), " s");
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


