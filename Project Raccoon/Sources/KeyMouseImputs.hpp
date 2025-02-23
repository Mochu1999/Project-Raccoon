#pragma once

#include "Globe.hpp"

//POR QUE COÑO NO REQUIERE INCLUDES?


int counter = 5;

//Pointers can be re-seated while references cannot. But we are not re seating anything so whatever
struct AllPointers {
	//BinariesManager* binariesManager;
	Lines2D* lines2D;
	Camera* camera;

	AllPointers( Camera* c, Lines2D* l):camera(c),lines2D(l){}
};

//The standard is to use callbacks for one-time event (typing, increase something once per press) and another function
//  for per-frame polling that updates each frame or based on deltaTime for smooth changes in a variable over frames

void keyboardEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	AllPointers* allPointers = static_cast<AllPointers*>(glfwGetWindowUserPointer(window));
	Camera* camera = allPointers->camera;

	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_P:
			isRunning = !isRunning;
			print(isRunning);
			break;
		case GLFW_KEY_C:
			counter++;

			break;


			/*case GLFW_KEY_ESCAPE:
				if (binariesManager->currentProgramType == 1) {
					polygon->clear();
					polygon->addSet(model);
				}
				else if (binariesManager->currentProgramType == 0) {
					if (polygon->indices.size()) {
						polygon->clear();
						polygon->addSet(currentModel);
					}
					else {
						polygon->clear();
					}
				}
				counterI = 0;
				break;

			case GLFW_KEY_C:
				if (counterI < polygon->Points.size()) {
					polygon->sweepTriangulation();
					counterI++;
				}
				break;

			case GLFW_KEY_Q:
				switchOnePosition(model);
				break;

			case GLFW_KEY_SPACE:
				for (size_t i = 0; i < polygon->Points.size(); ++i) {
					polygon->sweepTriangulation();
				}
				printv2(polygon->positions);
				printflat3(polygon->indices);
				isRunning = true;
				break;

			case GLFW_KEY_O:
				binariesManager->readConfig();
				break;*/

		}

		///CTRL
		/*if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			switch (key)
			{
			case GLFW_KEY_N:

				binariesManager->writeConfig();

				if (binariesManager->currentProgramType == 1)
				{
					polygon->lines.cadMode = false;
					modelPositions = binariesManager->readModel();


					polygon->clear();
					model = convertPositions(modelPositions);
					polygon->addSet(model);

					isRunning = true;
				}
				else if (binariesManager->currentProgramType == 0)
				{
					polygon->clear();
					polygon->lines.cadAddSet(cursor);

					polygon->lines.cadMode = true;
					modelPositions.clear();


				}
				counterI = 0;

				break;

			case GLFW_KEY_S:
				auto oldFormatPositions = inverseTransforming(polygon->positions);
				printflat2(oldFormatPositions);
				binariesManager->writeModel(oldFormatPositions);
				break;

			}
		}*/
	}

	if (action == GLFW_PRESS || action == GLFW_REPEAT)
		switch (key) {
		case GLFW_KEY_W:
			camera->cameraPos += camera->forward * camera->translationSpeed;
			break;
			// etc...
		}

}



//keys functions gets triggered once per frame
void keyboardRealTimePolls(GLFWwindow* window, Camera& camera) {
	// Rotation


	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera.calculateForward(camera.forward, camera.rotationSpeed, camera.right);

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera.calculateForward(camera.forward, -camera.rotationSpeed, camera.right);

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera.calculateForward(camera.forward, -camera.rotationSpeed, camera.up);

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera.calculateForward(camera.forward, camera.rotationSpeed, camera.up);


	//translation
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.cameraPos = camera.cameraPos + camera.forward * camera.translationSpeed;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.cameraPos = camera.cameraPos - camera.forward * camera.translationSpeed;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.cameraPos = camera.cameraPos - camera.right * camera.translationSpeed;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.cameraPos = camera.cameraPos + camera.right * camera.translationSpeed;

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.cameraPos.y += camera.translationSpeed;

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		camera.cameraPos.y -= camera.translationSpeed;
}


//void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
////	AllPointers* allpointers = static_cast<AllPointers*>(glfwGetWindowUserPointer(window));
////
////	Polygons* polygon = allpointers->polygon;
////	BinariesManager* binariesManager = allpointers->binariesManager;
////
////
////
////	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
////	{
////		if (binariesManager->currentProgramType == binariesManager->CAD)
////		{
////			if (polygon->lines.cadMode && polygon->positions.size() > 2
////				&& polygon->positions.front() == polygon->positions.back())
////			{
////
////				polygon->lines.cadMode = false;
////
////				currentModel = polygon->positions;
////
////				polygon->clear();
////
////				polygon->addSet(currentModel);
////			}
////			else
////			{
////				if (polygon->lines.cadMode == false)
////				{
////					polygon->lines.cadMode = true;
////					polygon->lines.cadAddSet(cursor);
////				}
////
////
////
////				polygon->lines.cadAddSet(cursor);
////
////
////			}
////
////			//cout << "cursorX: " << cursorX << ", cursorY: " << cursorY << endl;
////			printv2(polygon->lines.positions);
////			printflat2(polygon->lines.indices);
////
////
////		}
////
////	}
//}







//An alternative in structure to keyboardRealTimePolls with the same functionality
//functions are binded to each key with lambda functions and in pollBindings if the key is pressed it gets executed
/*
struct KeyBinding {
	int key;
	std::function<void(Camera&)> onPress;
};


struct KeyBindingsManager {

	Camera& camera;
	std::vector<KeyBinding> keyBindings;

	KeyBindingsManager(Camera& cam)
		: camera(cam)
	{
		keyBindings = {
			//Rotation
			{ GLFW_KEY_UP,    [&](Camera& c) { c.calculateForward(c.forward,  c.rotationSpeed,   c.right); } },
			{ GLFW_KEY_DOWN,  [&](Camera& c) { c.calculateForward(c.forward, -c.rotationSpeed,   c.right); } },
			{ GLFW_KEY_RIGHT, [&](Camera& c) { c.calculateForward(c.forward, -c.rotationSpeed,   c.up); } },
			{ GLFW_KEY_LEFT,  [&](Camera& c) { c.calculateForward(c.forward,  c.rotationSpeed,   c.up); } },
			// Translation:
			{ GLFW_KEY_W,     [&](Camera& c) { c.cameraPos += c.forward * c.translationSpeed; } },
			{ GLFW_KEY_S,     [&](Camera& c) { c.cameraPos -= c.forward * c.translationSpeed; } },
			{ GLFW_KEY_A,     [&](Camera& c) { c.cameraPos -= c.right * c.translationSpeed; } },
			{ GLFW_KEY_D,     [&](Camera& c) { c.cameraPos += c.right * c.translationSpeed; } },
			{ GLFW_KEY_Q,     [&](Camera& c) { c.cameraPos.y += c.translationSpeed; } },
			{ GLFW_KEY_Z,     [&](Camera& c) { c.cameraPos.y -= c.translationSpeed; } },
		};
	}

	
	void pollBindings(GLFWwindow* window) {
		for (auto& binding : keyBindings) {
			if (glfwGetKey(window, binding.key) == GLFW_PRESS) {
				binding.onPress(camera);
			}
		}
	}


	
};
*/
//initialized as
//	KeyBindingsManager inputMgr(camera);
//	inputMgr.pollBindings(window);