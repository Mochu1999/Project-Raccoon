#pragma once

#include "Globe.hpp"
#include "mainMRS.hpp"
#include "Ship.hpp"
#include "SS.hpp"


int keyCounter = 5;

//Pointers can be re-seated while references cannot. But we are not re seating anything so whatever
struct AllPointers {
	Camera* camera;
	GlobalVariables* gv;
	MainMap* map;
	Ship* ship;
	SS* ss;

	AllPointers(Camera* camera_, GlobalVariables* gv_, MainMap* map_, Ship* ship_) :camera(camera_), map(map_), gv(gv_), ship(ship_) {}
};

//The standard is to use callbacks for one-time event (typing, increase something once per press) and another function
//  for per-frame polling that updates each frame or based on deltaTime for smooth changes in a variable over frames

void keyboardEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	AllPointers* allPointers = static_cast<AllPointers*>(glfwGetWindowUserPointer(window));
	Camera* camera = allPointers->camera;
	MainMap* map = allPointers->map;
	Ship* ship = allPointers->ship;
	GlobalVariables* gv = allPointers->gv;


	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_P:
			gv->isRunning = !gv->isRunning;
			print(gv->isRunning);
			break;
			/*case GLFW_KEY_C:
				keyCounter++;

				break;*/
		case GLFW_KEY_Q:
			if (gv->program == 1)
			{
				if (map->show)
					map->show = 0;
				else
					map->show = 1;
			}
			break;

			//cameraModes
		case GLFW_KEY_X:
			gv->cameraMode = drag;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			break;
		case GLFW_KEY_C:
			gv->cameraMode = FPS;
			gv->variationMPos = gv->mPos;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			break;
		case GLFW_KEY_V:
			gv->cameraMode = centered;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

			break;
		}


		//CTRL
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			switch (key)
			{
			case GLFW_KEY_1:
				gv->program = telemetry;
				ship->activateLight();

				break;
			case GLFW_KEY_2:
				gv->program = MRS;

				break;
			case GLFW_KEY_3:
				gv->program = solar;

				break;

			}

		}
	}
}



//keys functions gets triggered once per frame
void keyboardRealTimePolls(GLFWwindow* window, GlobalVariables& gv, Camera& camera, MainMap& map) {

	//TIENES ESTO PARTIDO ENTRE AQUÍ Y UPDATECAMERA

	// Rotation
	if (gv.program == telemetry || gv.program == solar)
	{
		if (gv.cameraMode == drag || gv.cameraMode == FPS)
		{
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

		
	}
	else if (gv.program == MRS)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			map.translationTotal -= {0, 10};

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			map.translationTotal += {0, 10};

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			map.translationTotal += {10, 0};

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			map.translationTotal -= {10, 0};
	}
}



void getPos(GLFWwindow* window, p2& mPos) {
	double xpos1, ypos1;
	glfwGetCursorPos(window, &xpos1, &ypos1);

	mPos = { (float)xpos1,(float)(windowHeight - ypos1) };
}

void mouseEventCallback(GLFWwindow* window, int button, int action, int mods) {
	AllPointers* allPointers = static_cast<AllPointers*>(glfwGetWindowUserPointer(window));
	GlobalVariables* gv = allPointers->gv;
	MainMap* map = allPointers->map;

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		gv->isLmbPressed = 1;
		gv->variationMPos = gv->mPos;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		gv->isLmbPressed = 0;
	}
}



void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	AllPointers* allPointers = static_cast<AllPointers*>(glfwGetWindowUserPointer(window));
	Camera* camera = allPointers->camera;
	GlobalVariables* gv = allPointers->gv;
	MainMap* map = allPointers->map;


	if (yoffset > 0)
	{
		if (gv->program == MRS)
		{
			map->totalPixels *= 1.15;
			map->update();
		}
		else if (gv->program == telemetry || gv->program == solar)
		{
			camera->cameraPos = camera->cameraPos + camera->forward * camera->translationSpeed*10;
		}
	}
	else if (yoffset < 0)
	{
		if (gv->program == MRS)
		{
			map->totalPixels /= 1.15;
			map->update();
		}
		else if (gv->program == telemetry || gv->program == solar)
		{
			camera->cameraPos = camera->cameraPos - camera->forward * camera->translationSpeed*10;
		}
	}

	else if (gv->program == telemetry || gv->program == solar)
	{

	}

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