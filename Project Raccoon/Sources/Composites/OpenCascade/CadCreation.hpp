#pragma once

#include "Common.hpp"
#include "ShapeRenderer.hpp"

struct CadCreation
{
	Shader& shader3D;
	Camera& camera;
	GlobalVariables& gv;

	ShapeRenderer& lastShape;

	Lines3D lines;
	p3 point1, point2;
	bool isPoint1 = 0;
	bool isExtruded = 0;
	ShapeRenderer shape,shape1; //intermediate shape

	CadCreation(Shader& shader3D_, Camera& camera_, GlobalVariables& gv_, ShapeRenderer& lastShape_)
		:shader3D(shader3D_), camera(camera_), gv(gv_), shape(gv, shader3D), shape1(gv, shader3D), lastShape(lastShape_)
	{

	}
	void draw()
	{


		if (gv.cadMode == polyline)
		{
			if (lines.positions.size()>=2)
			{
				lines.positions.back() = camera.cursorToXZPlane();
			}
			
			lines.draw();
			//print(lines.positions);
		}
		if (gv.cadMode == rectangle)
		{
			if (isPoint1 && point1!= camera.cursorToXZPlane())
			{
				shape.clear();
				point2 = camera.cursorToXZPlane();
				shape.addRectangle(point1, point2);
				shape.draw();
			}
		}
		if (gv.cadMode == circle)
		{
			if (isPoint1)
			{
				shape.clear();
				point2 = camera.cursorToXZPlane();
				shape.addCircle(point1, magnitude3(point2 - point1));
				shape.draw();
			}
		}
		if (gv.cadMode == sphere)
		{
			if (isPoint1 && point1 != camera.cursorToXZPlane())
			{
				shape.clear();
				shape1.clear();
				point2 = camera.cursorToXZPlane();
				shape.addCircle(point1, magnitude3(point2 - point1));
				shape1.addCircle(point1, magnitude3(point2 - point1),{0,0,1});
				shape.draw();
				shape1.draw();
			}
		}
		if (gv.cadMode == extrusion)
		{
			if (!isExtruded)
			{
				//It gets bugged if calculated in keyMouseInput, where it should be, with the callback
				point1.y = gv.mPos.y;
				isExtruded = 1;
			}

			shape.clear();
			shape.addShape(lastShape.shape);
			shape.extrudeFace((gv.mPos.y - point1.y)*0.12);
			shape.draw();

		}
	}
};