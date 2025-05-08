#include "AuxFunctionsOC.hpp"


#pragma once



struct ShapeRenderer
{
	TopoDS_Shape shape;

	GlobalVariables& gv;
	Shader& shader3D;

	unsigned int trVAO, VBO, trIBO, trNBO; //shared vertex buffer between tr and wire
	unsigned int wireVAO, wireIBO;

	std::vector<p3> positions, trNormals;
	std::vector<unsigned int> trIndices, wireIndices;

	ShapeRenderer(GlobalVariables& gv_, Shader& shader3D_) :gv(gv_), shader3D(shader3D_)
	{
		genBuffers();

	}

	void addIGES(const string iges)
	{
		clear();

		importIGES(iges, shape);
		
		extractShapeData();
		
		updateBuffers();
	}

	void addBoxShape(p3 corner, p3 size)
	{
		clear();

		gp_Pnt gpCorner(corner.x, corner.y, corner.z);
		shape = BRepPrimAPI_MakeBox(
			gpCorner,
			static_cast<Standard_Real>(size.x),
			static_cast<Standard_Real>(size.y),
			static_cast<Standard_Real>(size.z)
		);

		
	}

	void addSphereShape(p3 center, float radius)
	{
		clear();

		gp_Pnt gpCenter(center.x, center.y, center.z);
		shape = BRepPrimAPI_MakeSphere(gpCenter, static_cast<Standard_Real>(radius));

		extractShapeData();

		trNormals.clear();

		for (auto& pos : positions)
		{
			trNormals.push_back(normalize3(pos - center));
		}

		updateBuffers();
	}

	void genBuffers()
	{
		glGenVertexArrays(1, &trVAO);
		glGenVertexArrays(1, &wireVAO);

		glGenBuffers(1, &VBO);
		glGenBuffers(1, &trIBO);
		glGenBuffers(1, &wireIBO);
		glGenBuffers(1, &trNBO);

		glBindVertexArray(trVAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, trNBO);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);


		glBindVertexArray(wireVAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); //wireVAO must also be aware of the layout

		glBindVertexArray(0);
	}

	//Extracts the rendering data of the shape
	void extractShapeData()
	{
		int counter = 0;
		//Iterating through each face to get the rendering data
		for (TopExp_Explorer exp(shape, TopAbs_FACE); exp.More(); exp.Next())
		{
			const TopoDS_Face& face = TopoDS::Face(exp.Current());

			extractFaceData(face, positions, trIndices, wireIndices, trNormals);
			counter++;
			print(counter);
		}
	}

	void clear()
	{
		positions.clear();
		trIndices.clear();
		wireIndices.clear();
		trNormals.clear();
	}

	~ShapeRenderer()
	{
		glDeleteVertexArrays(1, &trVAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &trIBO);
		glDeleteBuffers(1, &trNBO);

		glDeleteVertexArrays(1, &wireVAO);
		glDeleteBuffers(1, &wireIBO);
	}


	//only at the start and after the data is modified
	//I am assuming the data does not usually change so I am not using subdata
	void updateBuffers()
	{
		glBindVertexArray(trVAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(p3), positions.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, trIndices.size() * sizeof(unsigned int), trIndices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, trNBO);
		glBufferData(GL_ARRAY_BUFFER, trNormals.size() * sizeof(p3), trNormals.data(), GL_STATIC_DRAW);

		glBindVertexArray(wireVAO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wireIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, wireIndices.size() * sizeof(unsigned int), wireIndices.data(), GL_STATIC_DRAW);
	}


	void draw() const
	{
		shader3D.bind();
		shader3D.setUniform("u_Model", gv.identityMatrix);
		shader3D.setUniform("u_Color", 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1);

		if (gv.visualizationMode == triangulated)
		{
			shader3D.setUniform("u_fragmentMode", 0);

			glBindVertexArray(trVAO);
			glDrawElements(GL_TRIANGLES, trIndices.size(), GL_UNSIGNED_INT, nullptr);

			glBindVertexArray(wireVAO);
			glDrawElements(GL_LINES, wireIndices.size(), GL_UNSIGNED_INT, nullptr);
		}
		else //(gv.visualizationMode == wire)
		{
			shader3D.setUniform("u_fragmentMode", 1);

			glBindVertexArray(wireVAO);
			glLineWidth(1);
			glDrawElements(GL_LINES, wireIndices.size(), GL_UNSIGNED_INT, nullptr);
			glLineWidth(1);
		}
	}
};


