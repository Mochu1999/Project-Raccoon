#pragma once

#include "Globe.hpp"
#include "Map.hpp"




int keyCounter = 5;

//Pointers can be re-seated while references cannot. But we are not re seating anything so whatever
struct AllPointers {
	Camera* camera;
	Map* map;
	GlobalVariables* gv;

	AllPointers(Camera* camera_, Map* map_, GlobalVariables* gv_) :camera(camera_), map(map_), gv(gv_) {}
};

//The standard is to use callbacks for one-time event (typing, increase something once per press) and another function
//  for per-frame polling that updates each frame or based on deltaTime for smooth changes in a variable over frames

void keyboardEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	AllPointers* allPointers = static_cast<AllPointers*>(glfwGetWindowUserPointer(window));
	Camera* camera = allPointers->camera;
	Map* map = allPointers->map;
	GlobalVariables* gv = allPointers->gv;

	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_P:
			gv->isRunning = !gv->isRunning;
			print(gv->isRunning);
			break;
		case GLFW_KEY_C:
			keyCounter++;

			break;
		case GLFW_KEY_S:
			if (map->show)
				map->show = 0;
			else
				map->show = 1;


			break;


		case GLFW_KEY_O:
			map->totalPixels *= 1.2;
			map->update();

			break;
		case GLFW_KEY_L:
			map->totalPixels /= 1.2;
			map->update();


			break;
		}


		//CTRL
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			switch (key)
			{
			case GLFW_KEY_1:
				if (gv->program == 1) gv->program = 0;
				else if (gv->program == 0) gv->program = 1;

				break;
			}
		}
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



void getPos(GLFWwindow* window, p2& mPos) {
	double xpos1, ypos1;
	glfwGetCursorPos(window, &xpos1, &ypos1);

	mPos = { (float)xpos1,(float)(windowHeight - ypos1) };
}

void mouseEventCallback(GLFWwindow* window, int button, int action, int mods) {
	AllPointers* allPointers = static_cast<AllPointers*>(glfwGetWindowUserPointer(window));
	Map* map = allPointers->map;

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		//print(mPos);


	}
}


void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	//AllPointers* allPointers = static_cast<AllPointers*>(glfwGetWindowUserPointer(window));
	//if (allPointers) {
	//	allPointers->scrollOffset = yoffset;  // Save scroll value
	//}
}




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