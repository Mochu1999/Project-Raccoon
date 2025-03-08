#pragma once

struct Overlay2D {

	Shader& shader2D;
	Camera& camera;

	Polygons2D background;

	Overlay2D(Shader& shader2D_, Camera& camera_) :shader2D(shader2D_), camera(camera_) {

		background.addSet({ {windowWidth / 2,0}, {windowWidth,0},{windowWidth,windowHeight}
							,{windowWidth/2,windowHeight},{windowWidth / 2,0} });
	}

	void draw() {
		shader2D.bind();
		transparent();
		shader2D.setUniform("u_Model2D", camera.identityMatrix);
		shader2D.setUniform("u_Color", 0.035f, 0.065f, 0.085f, 1.0f);

		background.draw();
	}
};