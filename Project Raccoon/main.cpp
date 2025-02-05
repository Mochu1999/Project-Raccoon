

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

#include "Text.hpp"

#include "auxiliary_elements.hpp"

using namespace std::chrono;

// to not render what is not visible to the camera:
//glEnable(GL_CULL_FACE);       // Enable face culling
//glCullFace(GL_BACK);          // Cull back faces
//glFrontFace(GL_CCW);          // Counter-clockwise winding is front-facing



struct AllPointers {
	BinariesManager* binariesManager;
	Polygons* polygon;
};



float crtRotation=0, objtRotation;

void orderRudder(vector<p3>& positions) {
	cout << "Rotame esta: ";
	cin >> objtRotation;
	crtRotation = objtRotation - crtRotation;
	cout << "rotating "<< crtRotation << endl;
	rotate3D(positions, 0, crtRotation, 0);
	cout << "chama " << endl;
}





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






	Shader shader3D("resources/shaders/shader3D.shader");
	Shader shader2D("resources/shaders/shader2D.shader");
	Shader shaderText("resources/shaders/shaderText.shader");
	//shader3D.bind();


	Camera camera(window);

	Settings settings(camera);




	Sphere light(3, 80);
	p3 lightPos = { 30,25,40 };
	light.addSet(lightPos);








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



	Polygons2D polygon2D; //the blue square

	Polyhedra stl, timon, modelo;
	{
		//readSTL(stl.positions, stl.normals, "modeloNR.stl");
		//stl.simpleIndices();
		//writeSimplePolyhedra(stl.positions, stl.normals, stl.indices);
	}


	readSimplePolyhedra(timon.positions, timon.normals, timon.indices, "timon.bin");
	readSimplePolyhedra(modelo.positions, modelo.normals, modelo.indices, "modelo.bin");





	//getPos(window);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Look for uniform buffer objects. Used to send a lot of uniforms into a program more efficiently. I think it let's you share DATA BETWEEN SHADERS
	int locationPerspective = glGetUniformLocation(shader3D.ID, "u_Perspective");
	int locationModel = glGetUniformLocation(shader3D.ID, "u_Model");
	int locationView = glGetUniformLocation(shader3D.ID, "u_View");
	int locationOrtho = glGetUniformLocation(shader2D.ID, "u_OrthoProjection");
	int locationOrthoText = glGetUniformLocation(shaderText.ID, "u_OrthoProjection");  //Y esta no tiene que hacerse estando binded??

	int locationFragment = glGetUniformLocation(shader3D.ID, "u_fragmentMode");


	int colorLocation3D = glGetUniformLocation(shader3D.ID, "u_Color");
	int colorLocation2D = glGetUniformLocation(shader2D.ID, "u_Color");

	int locationLightPos = glGetUniformLocation(shader3D.ID, "u_lightPos");
	int cameraPosition = glGetUniformLocation(shader3D.ID, "u_CamPos");

	shader3D.bind();
	glUniform3f(locationLightPos, lightPos.x, lightPos.y, lightPos.z);
	glUniformMatrix4fv(locationPerspective, 1, GL_FALSE, camera.perspectiveMatrix.data());

	glUniformMatrix4fv(locationModel, 1, GL_FALSE, camera.modelMatrix.data());


	//SEPARATE THE LOGIC OF THE KEYS FROM UPDATECAMERA
	camera.updateCamera();
	//glUniform3f(cameraPosition, camera.cameraPos.x, camera.cameraPos.y, camera.cameraPos.z);
	//glUniformMatrix4fv(locationMVP, 1, GL_FALSE, camera.vpMatrix.data());
	/*glUniformMatrix4fv(locationOrtho, 1, GL_FALSE, camera.orthoMatrix.data());
	glUniformMatrix4fv(locationOrthoText, 1, GL_FALSE, camera.orthoMatrix.data());*/

	//shaderText.bind(); //NO DEBERÍA DE NECESITAR ESTAR BINDED?
	Text text("abcp 100,200.521 Lorem ipsum dolor sit amet, consectetur adipiscing elit.", 100, 100);
	shader2D.bind();
	glUniformMatrix4fv(locationOrtho, 1, GL_FALSE, camera.orthoMatrix.data());
	shaderText.bind();
	glUniformMatrix4fv(locationOrthoText, 1, GL_FALSE, camera.orthoMatrix.data());

	float angle = 0;
	//glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetKeyCallback(window, keyCallback);

	//system("cls");
	while (!glfwWindowShouldClose(window))
	{
		if (isRunning)
		{
			shader3D.bind();

			//opaque objects first
			glEnable(GL_DEPTH_TEST);		//ESTO PUEDE IR ANTES DEL BIND?
			glDepthMask(GL_TRUE);      //depth writing
			glDisable(GL_BLEND);

			glClearColor(0.035f, 0.065f, 0.085f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



			glUniform1i(locationFragment, 1);
			//axis
			glUniform4f(colorLocation3D, 1.0, 0.0, 0.0, 1.0);
			xLine.draw();
			glUniform4f(colorLocation3D, 0.0, 1.0, 0.0, 1.0);
			yLine.draw();
			glUniform4f(colorLocation3D, 0.0, 0.0, 1.0, 1.0);
			zLine.draw();

			//light
			glUniform4f(colorLocation3D, 1, 1, 1, 1.0);
			light.draw();



			glRotatef(0.0, 0.0, 45.0, 1.0);


			
			//model
			glUniform1i(locationFragment, 0);

			camera.modelMatrix = camera.createModelMatrix({ 20,0,0 },angle,{1,0,0});
			//angle++;
			glUniformMatrix4fv(locationModel, 1, GL_FALSE, camera.modelMatrix.data());
			glUniform4f(colorLocation3D, 255.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f, 1);
			timon.draw();
			glUniform4f(colorLocation3D, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1);
			modelo.draw();
			camera.modelMatrix = camera.identityMatrix;
			glUniformMatrix4fv(locationModel, 1, GL_FALSE, camera.modelMatrix.data());

			


			////transparent objects
			//glEnable(GL_BLEND);
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			//glDepthMask(GL_FALSE);
			//glUniform1i(locationFragment, 0);
			//glUniform4f(colorLocation3D, 40.0f / 255.0f, 239.9f / 255.0f, 239.0f / 255.0f, 0.6);





			///2d objects
			shader2D.bind();
			glUniform4f(colorLocation2D, 40.0f / 255.0f, 239.9f / 255.0f, 239.0f / 255.0f, 0.6);
			polygon2D.draw();




			//text
			glDisable(GL_DEPTH_TEST);
			shaderText.bind();
			text.draw();

			
			




			shader3D.bind();
			
			if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
				orderRudder(timon.positions);

			camera.updateKeys();
			camera.updateCamera();
			glUniform3f(cameraPosition, camera.cameraPos.x, camera.cameraPos.y, camera.cameraPos.z);
			glUniformMatrix4fv(locationView, 1, GL_FALSE, camera.viewMatrix.data());
			

			

			

			/*print(camera.forward);
			print(camera.cameraPos);*/

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


