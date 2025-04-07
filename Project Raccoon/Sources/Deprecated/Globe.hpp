#pragma once


#include "Common.hpp"
//en lonLatToMercator hay un float(PI), hay que modificar PI
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

//// .x is longitude, .y is latitude
//vector<p2> lonLat(const vector<p3> positions_) {
//	vector<p2> positions;
//
//	for (size_t i = 0; i < positions_.size(); i++)
//	{
//		positions.push_back({ atan2f(positions_[i].z, positions_[i].x) ,asinf(positions_[i].y) });
//	}
//	return positions;
//}


float earthRadius = 6378137.0f;
//Web mercator projection. Outoput in mercator projected meters
// In the equator the distances are exact (cilindrical proj) but you lose accuracy the more away you are from it
// Mercator is only valid for visualization, otherwise use geodesic calculations
//assumes you wont have latitudes close to +-90 for now. lonlats in degrees
vector<p2> lonLatToMercator(const vector<p2> lonLats) {
	vector<p2> positions;
	positions.reserve(lonLats.size());

	for (auto& ll : lonLats)
	{
		float lambda = radians(ll.x);   // lon in radians
		float phi = radians(ll.y);   // lat in radians



		positions.push_back({ earthRadius * lambda, earthRadius * log(tan((3.14159265359f / 4.0f) + (phi / 2.0f))) });
	}

	return positions;
}

p2 lonLatToMercator(const p2 lonLat) {

	float lambda = radians(lonLat.x);   // lon in radians
	float phi = radians(lonLat.y);   // lat in radians

	return { earthRadius * lambda, earthRadius * log(tan((3.14159265359f / 4.0f) + (phi / 2.0f))) };
}


//Comprobar
vector<p2> mercatorToLonLat(const vector<p2> coords) {
	vector<p2> lonLats;
	lonLats.reserve(coords.size());

	for (auto& coord : coords) {
		// Convertir x (Mercator) a longitud (lambda)
		float lambda = coord.x / earthRadius;

		// Convertir y (Mercator) a latitud (phi)
		float y = coord.y / earthRadius;
		float phi = 2.0f * atan(exp(y)) - PI / 2.0f;

		// Convertir radianes a grados
		float lon = lambda * 180.0f / PI;
		float lat = phi * 180.0f / PI;

		lonLats.push_back({ lon, lat });
	}

	return lonLats;
}

//Comprobar
p2 mercatorToLonLat(const p2 pos) {


	// Convertir x (Mercator) a longitud (lambda)
	float lambda = pos.x / earthRadius;

	// Convertir y (Mercator) a latitud (phi)
	float y = pos.y / earthRadius;
	float phi = 2.0f * atan(exp(y)) - PI / 2.0f;

	// Convertir radianes a grados
	float lon = lambda * 180.0f / PI;
	float lat = phi * 180.0f / PI;



	return { lon, lat };
}

//the map format that gives you longitudes and latitudes is WGS84 (EPSG:4326)