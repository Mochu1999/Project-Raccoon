

#include "Common.hpp"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"

#include "lines3d.hpp"
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

#include "Text.h"

// to not render what is not visible to the camera:
//glEnable(GL_CULL_FACE);       // Enable face culling
//glCullFace(GL_BACK);          // Cull back faces
//glFrontFace(GL_CCW);          // Counter-clockwise winding is front-facing




using namespace std::chrono;



struct AllPointers {
	BinariesManager* binariesManager;
	Polygons* polygon;
};










int main(void)
{
	GLFWwindow* window = initialize();

	/*glDebugMessageCallback(MessageCallback, nullptr);
	glEnable(GL_DEBUG_OUTPUT);*/


	//BinariesManager binariesManager;




	//writeModel(model);
	model = readModel("gato.bin");
	Polygons polygon;
	polygon.addSet(model);


	//print(polygon.positions);
	//print(polygon.indices);

	/*Polyhedra stl, bin;

	TimeCounter tc1;
	readSTL(stl.positions, stl.normals, "Eiffel.STL");
	tc1.endCounter();

	stl.simpleIndices();


	TimeCounter tc2;
	readSimplePolyhedra(bin.positions, bin.normals, bin.indices, "Eiffel.bin");
	tc2.endCounter();

	print( tc1.endTime/ tc2.endTime);*/


	Sphere auxSphere(1);
	auxSphere.addSet(polygon.centroid);

	//printv3(model);

	Auxiliary_Elements groundLines;


	Shader shader("resources/shaders/shader1.shader");
	shader.Bind();


	Camera camera(window);


	Settings settings(camera);

	Fourier fourier;


	//WettedSurface wettedSurface(polygon,fourier);


	Pyramid pyramid;

	Sphere light(3, 80);
	p3 lightPos = { 30,50,25 };
	light.addSet(lightPos);


	Sphere sphere(40, 10);
	sphere.addSet({ 00,00,-80 });


	Arrows arrows;
	arrows.addSet(fourier.positions, fourier.normals);




	Lines3d xLine2;
	xLine2.addSet({ {0,0,0},{5,0,0} });
	Lines3d xLine;
	xLine.addSet({ {-100,0,0},{100,0,0} });
	Lines3d yLine;
	yLine.addSet({ {0,-100,0},{0,100,0} });
	Lines3d zLine;
	zLine.addSet({ {0,0,-100},{0,0,100} });
	Lines3d zLine2;
	zLine2.addSet({ {0,0,0},{0,0,5} });



	Polygons2D p2d;




	printm16(camera.orthoMatrix);



	Polyhedra stl, bin;


	/*readSTL(stl.positions, stl.normals, "Eiffel.STL");


	stl.simpleIndices();*/



	readSimplePolyhedra(bin.positions, bin.normals, bin.indices, "Eiffel.bin");







	//getPos(window);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	int locationMVP = glGetUniformLocation(shader.ID, "u_MVP");
	int locationOrtho = glGetUniformLocation(shader.ID, "u_OrthoProjection");

	int locationFragment = glGetUniformLocation(shader.ID, "u_fragmentMode");
	int locationVertex = glGetUniformLocation(shader.ID, "u_vertexMode");

	int colorLocation = glGetUniformLocation(shader.ID, "u_Color");

	int locationLightPos = glGetUniformLocation(shader.ID, "u_lightPos");
	int cameraPosition = glGetUniformLocation(shader.ID, "camPos");

	//glUniform3f(locationLightPos, lightPos.x, lightPos.y, lightPos.z);





	//SEPARATE THE LOGIC OF THE KEYS FROM UPDATECAMERA
	camera.updateCamera();
	glUniform3f(cameraPosition, camera.cameraPos.x, camera.cameraPos.y, camera.cameraPos.z);
	glUniformMatrix4fv(locationMVP, 1, GL_FALSE, camera.vpMatrix.data());
	glUniformMatrix4fv(locationOrtho, 1, GL_FALSE, camera.orthoMatrix.data());


	Text text("abcp 100,200.521", 100, 100);


	int counter = 0;


	//glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetKeyCallback(window, keyCallback);


	while (!glfwWindowShouldClose(window))
	{
		//break;

		//system("cls");
		if (isRunning)
		{
			//opaque objects first
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_TRUE);      //depth writing
			glDisable(GL_BLEND);

			glClearColor(0.035f, 0.065f, 0.085f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



			glUniform1i(locationFragment, 1);
			glUniform1i(locationVertex, 1);

			glUniform4f(colorLocation, 1.0, 1.0, 1.0, 1.0);
			//groundLines.draw();

			glUniform4f(colorLocation, 1.0, 0.0, 0.0, 1.0);
			xLine.draw();
			glUniform4f(colorLocation, 0.0, 1.0, 0.0, 1.0);
			yLine.draw();
			glUniform4f(colorLocation, 0.0, 0.0, 1.0, 1.0);
			zLine.draw();



			glUniform4f(colorLocation, 1.0, 0.0, 1.0, 1);
			xLine2.draw();
			zLine2.draw();
			//wettedSurface.draw();

			//line.draw();

			//triangles.lines.draw();
			//triangles.draw();

			/*trLines.draw();
			auxSquare.draw();*/







			glUniform1i(locationFragment, 0);


			glUniform4f(colorLocation, 40.0f / 255.0f, 239.9f / 255.0f, 239.0f / 255.0f, 1);

			sphere.draw();
			fourier.draw();
			//eiffel.draw();
			//fourier.createWavePositions();
			glUniform4f(colorLocation, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1);
			bin.draw();
			//printflat(fourier.indices);
			glUniform4f(colorLocation, 187.0f / 255.0f, 165.61f / 255.0f, 61.0f / 255.0f, 1);
			//bin.draw();
			//pyramid.draw();
			glUniform4f(colorLocation, 135.0f / 255.0f, 0.0, 0.0, 1);
			glUniform4f(colorLocation, 255 / 255.0f, 0.0, 0.0, 1);
			//polygon.draw();

			glUniform1i(locationFragment, 1);
			glUniform4f(colorLocation, 0, 0, 1.0, 1.0);
			//for (auto& line : fourier.lines)
			//{
			//	line.draw();
			//}
			glUniform4f(colorLocation, 1, 0, 0, 1.0);
			arrows.draw();

			glUniform4f(colorLocation, 1, 1, 1, 1.0);
			light.draw();
			//auxSphere.draw();
			//polygon.lines.draw();

			//transparent objects
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDepthMask(GL_FALSE);

			glUniform1i(locationFragment, 0);
			glUniform4f(colorLocation, 40.0f / 255.0f, 239.9f / 255.0f, 239.0f / 255.0f, 0.6);




			//sphere.drawLines();
			//sphere.draw();

			/////2d objects
			glUniform1i(locationVertex, 0);
			glUniform1i(locationFragment, 1);
			glUniform4f(colorLocation, 40.0f / 255.0f, 239.9f / 255.0f, 239.0f / 255.0f, 0.6);
			p2d.draw();



			//text
			glUniform1i(locationVertex, 2);
			glUniform1i(locationFragment, 3);
			glDisable(GL_DEPTH_TEST);
			text.draw();




			camera.updateKeys();
			camera.updateCamera();
			glUniform3f(cameraPosition, camera.cameraPos.x, camera.cameraPos.y, camera.cameraPos.z);
			glUniformMatrix4fv(locationMVP, 1, GL_FALSE, camera.vpMatrix.data());
			glUniformMatrix4fv(locationOrtho, 1, GL_FALSE, camera.orthoMatrix.data());


			//printp3(camera.forward);
			//printp3(camera.cameraPos);

			//break;
		}
		//isRunning = false;
		settings.write();

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


