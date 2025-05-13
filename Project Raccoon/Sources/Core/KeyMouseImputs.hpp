﻿#pragma once

#include "Globe.hpp"
#include "mainMRS.hpp"
#include "Light.hpp"
#include "SS.hpp"
#include "MainOC.hpp"

int keyCounter = 5;

//Pointers can be re-seated while references cannot. But we are not re seating anything so whatever
struct AllPointers {
	Camera* camera;
	GlobalVariables* gv;
	MainMap* map;
	Light* ship;
	SS* ss;
	MainOC* mainOC;

	AllPointers(Camera* camera_, GlobalVariables* gv_, MainMap* map_, Light* ship_, MainOC* mainOC_)
		:camera(camera_), map(map_), gv(gv_), ship(ship_), mainOC(mainOC_) {}
};

//The standard is to use callbacks for one-time event (typing, increase something once per press) and another function
//  for per-frame polling that updates each frame or based on deltaTime for smooth changes in a variable over frames


void keyboardEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	AllPointers* allPointers = static_cast<AllPointers*>(glfwGetWindowUserPointer(window));
	Camera* camera = allPointers->camera;
	MainMap* map = allPointers->map;
	Light* ship = allPointers->ship;
	GlobalVariables* gv = allPointers->gv;
	MainOC* mainOC = allPointers->mainOC;



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

			//cameraModes //updateCamera in camera.cpp also need to be updated if ths is to change
		case GLFW_KEY_X:
			if (gv->program == telemetry || gv->program == solar || gv->program == openCascade)
			{
				gv->cameraMode = drag;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			break;
		case GLFW_KEY_C:
			if (gv->program == telemetry || gv->program == solar || gv->program == openCascade)
			{
				gv->cameraMode = FPS;
				gv->LastLMPos = gv->mPos;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			break;
		case GLFW_KEY_V:
			if (gv->program == telemetry || gv->program == solar || gv->program == openCascade)
			{
				gv->cameraMode = centered;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

			}
			break;
		case GLFW_KEY_O:
			if (gv->program == openCascade)
			{
				gv->visualizationMode = triangulated;
			}
			break;
		case GLFW_KEY_I:
			if (gv->program == openCascade)
			{
				gv->visualizationMode = wire;


			}
			break;
		case GLFW_KEY_T:
			if (gv->program == openCascade)
			{
				if (gv->cadMode == polyline)
				{
					gv->cadMode = none;
				}
				else if (gv->cadMode != polyline)
				{
					gv->cadMode = polyline;
				}
			}
			break;
		case GLFW_KEY_Y:
			if (gv->program == openCascade)
			{
				if (gv->cadMode == rectangle)
				{
					gv->cadMode = none;
				}
				else if (gv->cadMode != rectangle)
				{
					gv->cadMode = rectangle;
				}
			}
			break;
		case GLFW_KEY_G:
			if (gv->program == openCascade)
			{
				if (gv->cadMode == circle)
				{
					gv->cadMode = none;
				}
				else if (gv->cadMode != circle)
				{
					gv->cadMode = circle;
				}
			}
			break;
		case GLFW_KEY_H:
			if (gv->program == openCascade)
			{
				if (gv->cadMode == sphere)
				{
					gv->cadMode = none;
				}
				else if (gv->cadMode != sphere)
				{
					gv->cadMode = sphere;
				}
			}
			break;
		case GLFW_KEY_U:
			if (gv->program == openCascade)
			{
				if (gv->cadMode == extrusion)
				{
					gv->cadMode = none;
				}
				else if (gv->cadMode != extrusion)
				{
					gv->cadMode = extrusion;

					camera->setCursorToXZPoint(mainOC->cadCreator.lastShape.positions[0]);
					
				}
			}
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
			case GLFW_KEY_4:
				gv->program = openCascade;

				break;
			case GLFW_KEY_O: //changing maps
				if (gv->program == MRS)
				{

				}
				if (gv->program == openCascade)
				{
					mainOC->a.openShape();
				}
				break;
			case GLFW_KEY_S:
				if (gv->program == MRS)
				{
				}
				if (gv->program == openCascade)
				{
					mainOC->a.saveShape();
				}
				break;
			}

		}
	}
}



//keys functions gets triggered once per frame
void keyboardRealTimePolls(GLFWwindow* window, GlobalVariables& gv, Camera& camera, MainMap& map) {

	//TIENES ESTO PARTIDO ENTRE AQUÍ Y UPDATECAMERA

	// Rotation
	if (gv.program == telemetry || gv.program == solar || gv.program == openCascade)
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
	MainOC* mainOC = allPointers->mainOC;
	Camera* camera = allPointers->camera;

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		gv->isLmbPressed = 1;
		gv->LastLMPos = gv->mPos;

		if (gv->cadMode == polyline)
		{
			gv->cadPositions.back() = camera->cursorToXZPlane();
			gv->cadPositions.push_back(camera->cursorToXZPlane());


			if (gv->cadPositions.size() >= 2)
			{
				mainOC->cadCreator.lines.clear();
				mainOC->cadCreator.lines.addSet(gv->cadPositions);

			}
		}
		if (gv->cadMode == rectangle)
		{
			if (mainOC->cadCreator.isPoint1 == 0)
			{
				mainOC->cadCreator.point1 = camera->cursorToXZPlane();
				mainOC->cadCreator.isPoint1 = 1;
			}
			else
			{
				mainOC->a.addRectangle(mainOC->cadCreator.point1, camera->cursorToXZPlane());
				mainOC->cadCreator.isPoint1 = 0;
				gv->cadMode = none;
			}
		}
		if (gv->cadMode == circle)
		{
			if (mainOC->cadCreator.isPoint1 == 0)
			{
				mainOC->cadCreator.isPoint1 = 1;
				mainOC->cadCreator.point1 = camera->cursorToXZPlane();
			}
			else
			{
				mainOC->a.addCircle(mainOC->cadCreator.point1,magnitude3(camera->cursorToXZPlane() - mainOC->cadCreator.point1));
				mainOC->cadCreator.isPoint1 = 0;
				gv->cadMode = none;
			}
		}
		if (gv->cadMode == sphere)
		{
			if (mainOC->cadCreator.isPoint1 == 0)
			{
				mainOC->cadCreator.point1 = camera->cursorToXZPlane();
				mainOC->cadCreator.isPoint1 = 1;
			}
			else
			{
				mainOC->a.addSphereShape(mainOC->cadCreator.point1, magnitude3(camera->cursorToXZPlane() - mainOC->cadCreator.point1));
				mainOC->cadCreator.isPoint1 = 0;
				gv->cadMode = none;
			}
		}
		if (gv->cadMode == extrusion)
		{
			mainOC->a.extrudeFace((gv->mPos.y - mainOC->cadCreator.point1.y) * 0.12);
			mainOC->cadCreator.isExtruded = 0;
			gv->cadMode = none;


		}
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		gv->isLmbPressed = 0;
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{

		if (gv->cadMode == polyline)
		{
			gv->cadPositions.back() =gv->cadPositions[0];
			mainOC->cadCreator.lines.clear();
			mainOC->cadCreator.lines.addSet(gv->cadPositions);

			mainOC->a.addPolyline(gv->cadPositions);
			gv->cadPositions.clear();
			gv->cadMode = none;
			camera->setCursorToXZPoint(mainOC->a.positions[0]);


		}
	}



	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
	{
		gv->isMmbPressed = 1;
		gv->LastMMPos = gv->mPos;
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
	{
		gv->isMmbPressed = 0;
	}
}


float scrollTranslationSpeedFactor = 20;

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
		else if (gv->program == telemetry || gv->program == solar || gv->program == openCascade)
		{
			camera->cameraPos = camera->cameraPos + camera->forward * camera->translationSpeed * scrollTranslationSpeedFactor;
		}
	}
	else if (yoffset < 0)
	{
		if (gv->program == MRS)
		{
			map->totalPixels /= 1.15;
			map->update();
		}
		else if (gv->program == telemetry || gv->program == solar || gv->program == openCascade)
		{
			camera->cameraPos = camera->cameraPos - camera->forward * camera->translationSpeed * scrollTranslationSpeedFactor;
		}
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