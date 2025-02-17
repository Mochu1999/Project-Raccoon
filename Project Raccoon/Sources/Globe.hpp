#pragma once


#include "Common.hpp"
//en lonLatTo2D hay un float(pi), hay que modificar PI
//RESERVES

//QUITO EL ÚLTIMO PUNTO PARA QUE NO DE LA VUELTA ENTERA

//Horizontal lines of a unity globe. Radians
//For the middle line Starts creating in -PI (-1,0,0) and ends in +PI (-1,0,0) to have the 0 centered (1,0,0). 
vector<p3> createLatitude(float angleLatitude, int points = 100) {

	vector<p3> positions;
	float angleBetweenLongitudes = 2.0f * PI / points;

	//Height of the xz plane
	float y = sin(angleLatitude);

	//the distance from the OY axis to all the circle points
	float r = cos(angleLatitude);

	for (int i = 0; i <= points; i++) 
	{
		float lon = -PI + i * angleBetweenLongitudes;
		positions.push_back({ r * cos(lon), y, r * sin(lon) });
	}

	return positions;
}


//Vertical lines of a globe. 
//For the middle line starts and ends in the same point as createLatitudes
vector<p3> createLongitude(float angleLongitude, int points = 100) {

    vector<p3> positions;
    float angleBetweenLatitudes = 2.0f * PI / points;

    for (int i = 0; i <= points; i++) {
        float lat = -PI + i * angleBetweenLatitudes;

        float x = cos(lat) * cos(angleLongitude);
        float y = sin(lat);
        float z = cos(lat) * sin(angleLongitude);

        positions.push_back({ x, y, z });
    }
    return positions;
}

// .x is longitude, .y is latitude
vector<p2> lonLat(const vector<p3> positions_) {
	vector<p2> positions;

	for (size_t i = 0; i < positions_.size(); i++)
	{
		positions.push_back({ atan2f(positions_[i].z, positions_[i].x) ,asinf(positions_[i].y) });
	}
	return positions;
}

float maxLat = radians(85);
float earthRadius = 6378137.0f;
vector<p2> lonLatTo2D(const vector<p2>& lonLats) {
	vector<p2> positions;
	positions.reserve(lonLats.size());

	for (auto& ll: lonLats)
	{
		float lambda = ll.x;   // lon in radians
		float phi = ll.y;   // lat in radians

		// clamp lat
		if (phi > maxLat) phi = maxLat;
		if (phi < -maxLat) phi = -maxLat;

		//positions.push_back({ earthRadius * lambda, earthRadius * log(tan((3.14159265359f / 4.0f) + (phi / 2.0f))) });
		//positions.back() /= 10E+4;
		//print(ll);
		positions.push_back(ll);
		positions.back() *= 25;
		positions.back().x += 500;
		positions.back().y += -300;
	}

	return positions;
}

//the map format that gives you longitudes and latitudes is WGS84 (EPSG:4326)