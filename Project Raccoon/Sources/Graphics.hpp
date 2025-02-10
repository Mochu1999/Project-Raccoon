#pragma once
#include "Common.hpp"
#include "Circles.hpp"

struct Graphics {

};


vector<p2> createRoundedSquare(float length, float roundedPercentage, p2 startingPos = { 200,200 }) {
	vector<p2> positions;
	//reserve

	//starting arbitrarily on the bottom left corner
	//p2 startingPos = { 200,200 };

	float radius = length * roundedPercentage;

	vector<p2> temp = createArc(startingPos + radius, radius, radians(180), radians(270));
	positions.insert(positions.end(), temp.begin(), temp.end());

	temp = createArc({ startingPos.x + length - radius,startingPos.y + radius }, radius, radians(270), 0);
	positions.insert(positions.end(), temp.begin(), temp.end());

	temp = createArc(startingPos + (length - radius), radius, 0, radians(90));
	positions.insert(positions.end(), temp.begin(), temp.end());

	temp = createArc({ startingPos.x + radius,startingPos.y + length - radius }, radius, radians(90), radians(180));
	positions.insert(positions.end(), temp.begin(), temp.end());

	positions.emplace_back(positions.front());

	return positions;
}