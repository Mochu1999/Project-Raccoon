#pragma once


void calculateBoundingBox(vector<std::pair<p2, p2>>& boundingBoxes, const vector<vector<p2>>& lonLatPositions)
{
	boundingBoxes.clear();
	boundingBoxes.reserve(lonLatPositions.size());

	for (auto& vec : lonLatPositions)
	{
		float minX = std::numeric_limits<float>::max(), minY = std::numeric_limits<float>::max();
		float maxX = std::numeric_limits<float>::lowest(), maxY = std::numeric_limits<float>::lowest();

		for (auto& p : vec)
		{
			if (p.x < minX) minX = p.x;
			if (p.x > maxX) maxX = p.x;

			if (p.y < minY) minY = p.y;
			if (p.y > maxY) maxY = p.y;
		}

		boundingBoxes.push_back({ {minX,minY},{maxX,maxY} });
	}
}


//FUNCTION NEEDS TO BE HEAVILY TESTED. LOGIC OF COLLINEAR HAS NOT BEEN REVISED

//HOSTIAS, NECESITO QUE NO TOQUE EL PUNTO D O ME CREAR� 2 PUNTOS SI EN D CHOCAN DOS LINEAS

//wettedSurface.hpp has a calculateIntersectionPoint that works similarly but between p3s, a triangle and a lines, these are 2 2d lines
//Lines formed up by ab and cd 
bool isThereAnIntersectionPoint(const p2& a, const p2& b, const p2& c, const p2& d, p2& p)
{
	if (a == b || c == d) return false;
	
	//checking if they bounding boxes collide for an early exit
	if (std::max(a.x, b.x) < std::min(c.x, d.x) ||
		std::max(c.x, d.x) < std::min(a.x, b.x) ||
		std::max(a.y, b.y) < std::min(c.y, d.y) ||
		std::max(c.y, d.y) < std::min(a.y, b.y))
		return false; // bounding boxes do not overlap


	const float epsilon = 1e-6f;
	
	const p2 ab = { b.x - a.x ,b.y - a.y };
	const p2 cd = { d.x - c.x ,d.y - c.y };
	const p2 ac = { c.x - a.x ,c.y - a.y };

	/*
	In 2d geometry the cross product is a scalar value, not a vector, it represents the area of their parallelogram
	These are the possible cases:
	t = 0: Intersection at point a.
	0 < t < 1: Intersection between a and b (on the line segment).
	t = 1: Intersection at point b.
	t < 0: Intersection lies before a
	t > 1: Intersection lies beyond b
	*/


	const float precalculate = cross2(ab, cd);
	if (fabs(precalculate) < epsilon) { // parallel/colinear branch

		if (fabs(cross2(ab, ac)) > epsilon)
			return false; // parallel but not colinear

		// Project c and d onto ab as scalars
		float t0 = dot2(c - a, ab);
		float t1 = dot2(d - a, ab);
		float denom = dot2(ab, ab);

		if (denom < epsilon) return false; // ab is too small � treat as no segment

		t0 /= denom;
		t1 /= denom;

		// Ensure t0 <= t1
		if (t0 > t1) std::swap(t0, t1);

		// Check overlap in [0, 1]
		if (t1 < 0 || t0 > 1)
			return false;

		// Return start of overlap (clamped to ab)
		float t = std::max(0.0f, t0);
		p = a + ab * t;
		return true;
	}

	float t = cross2(ac, cd) / precalculate; // (ac�cd)/(ab�cd)
	float u = cross2(ac, ab) / precalculate; // (ac�ab)/(ab�cd)

	if (t >= 0 && t <= 1 && u >= 0 && u <= 1) { // Return false for intersection in d
		p.x = a.x + t * ab.x;
		p.y = a.y + t * ab.y;
		return true;
	}

	return false;
}