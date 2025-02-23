#pragma once

#include "Common.hpp"



struct Camera {

	float rotationSpeed = 0.05f;
	float translationSpeed = 0.3f;

	GLFWwindow* window;

	
	float fov = 45.0f * PI / 180;
	float aspectRatio = windowWidth / windowHeight;
	float nearZ = 0.1f;
	float farZ = 3000.0f;

	std::array<float, 16> perspectiveMatrix;
	std::array<float, 16> viewMatrix;
	std::array<float, 16> modelMatrix;
	std::array<float, 16> model2DMatrix;
	std::array<float, 16> identityMatrix;
	std::array<float, 16> vpMatrix;

	std::array<float, 16> orthoMatrix;

	p3 cameraPos;

	//p3 forward = { 0.0f,-1.0f,0.0f }; //cuando dejó esto de funcionar?
	p3 forward = normalize3(p3{ 0.148691,-0.742031,-0.651083 });
	p3 right; //0 because are gettin recalculated anyways
	p3 up;


	Camera(GLFWwindow* window_) :window(window_) {

		orthoMatrix = createOrthoMatrix();
		perspectiveMatrix = createPerspectiveMatrix();

		p3 objPos = { 5.0f, 2.0f, 1.0f };
		float angle = 3.14159f * 0.25f; // 45 degrees in radians
		p3 rotAxis = { 0.0f, 1.0f, 0.0f }; // Y axis

		//modelMatrix = createModelMatrix(objPos, angle, rotAxis);
		identityMatrix = createIdentityMatrix();
		modelMatrix = identityMatrix;

	}


	std::array<float, 16> createScalingMatrix(float scaleX, float scaleY, float scaleZ);


	array<float, 16> createOrthoMatrix();

	////Perspective and View
	array<float, 16> createPerspectiveMatrix();

	std::array<float, 16> createViewMatrix(const p3& right, const p3& up, p3 forward, const p3& cameraPos);

	//AQUÍ PONDRÍA TRANSLATION EN VEZ DE POSITION Y CREARÍA MUCHAS FUNCIONES CON DISTINTOS INPUT
	std::array<float, 16> createModelMatrix(const p3& position);
	std::array<float, 16> createModelMatrix(const p3 position, float angleDeg, p3 axis, float scale = 1);
	std::array<float, 16> createIdentityMatrix();

	std::array<float, 16> createModel2DMatrix(const p2 position, float angleDeg, float scale) {
		std::array<float, 16> modelMatrix = createIdentityMatrix(); // Assume this returns a 4x4 identity matrix
		float theta = radians(angleDeg);
		float c = std::cos(theta);
		float s = std::sin(theta);

		
		// Column 0: [scale * cosθ, scale * sinθ, 0, 0]
		modelMatrix[0] = scale * c;
		modelMatrix[1] = scale * s;

		// Column 1: [-scale * sinθ, scale * cosθ, 0, 0]
		modelMatrix[4] = -scale * s;
		modelMatrix[5] = scale * c;

		// Column 2: [0, 0, 1, 0] 

		// Column 3: [translation.x, translation.y, 0, 1]
		modelMatrix[12] = position.x;
		modelMatrix[13] = position.y;

		return modelMatrix;
	}


	//the rotations create a new forward vector and the other 2 are deduced from it
	void calculateForward(p3& forward, const float rotationSpeed, const p3& rotationAxis);





	//creates all the vector and updates the view and vp matrices
	void updateCamera();





	float xpos, ypos;
	double xpos1, ypos1;

	void getPos(GLFWwindow* window) {
		glfwGetCursorPos(window, &xpos1, &ypos1);
		ypos1 = windowHeight - ypos1;
		xpos = (float)xpos1;
		ypos = (float)ypos1;
	}
};

