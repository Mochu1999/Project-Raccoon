#include "AuxFunctionsOC.hpp"


#pragma once



struct ShapeRenderer
{
	TopoDS_Shape shape;





	ShapeRenderer(/*TopoDS_Shape shape_*/) /*: shape(shape_)*/
	{
		genBuffers();

		p3 corner = { 0, 0, 0 };
		p3 size = { 5, 3, 2 };

		//Hardcoding the shape for now
		gp_Pnt gpCorner(corner.x, corner.y, corner.z);

		TopoDS_Shape box = BRepPrimAPI_MakeBox
		(
			gpCorner,
			static_cast<Standard_Real>(size.x),
			static_cast<Standard_Real>(size.y),
			static_cast<Standard_Real>(size.z)
		);

		//These intermidiate variables are inefficient but I am interested in not changing extractFaceData() for now

		unsigned int trIndexOffset = 0;
		unsigned int wireIndexOffset = 0;
		for (TopExp_Explorer exp(box, TopAbs_FACE); exp.More(); exp.Next())
		{
			const TopoDS_Face& face = TopoDS::Face(exp.Current());

			std::vector<p3> interm_trPositions, interm_trNormals, interm_wirePositions;
			std::vector<unsigned int> interm_trIndices, interm_wireIndices;

			trIndexOffset = trPositions.size(); //Works bc a face won't have a repeated vertex
			wireIndexOffset = 0;
			extractFaceData(face, interm_trPositions, interm_trIndices, interm_trNormals, interm_wirePositions, interm_wireIndices
				, trIndexOffset, wireIndexOffset);

			trPositions.insert(trPositions.end(), interm_trPositions.begin(), interm_trPositions.end());
			trIndices.insert(trIndices.end(), interm_trIndices.begin(), interm_trIndices.end());
			trNormals.insert(trNormals.end(), interm_trNormals.begin(), interm_trNormals.end());

			wirePositions.insert(wirePositions.end(), interm_wirePositions.begin(), interm_wirePositions.end());
			wireIndices.insert(wireIndices.end(), interm_wireIndices.begin(), interm_wireIndices.end());
		}

		updateBuffers();
	}
	unsigned int trVAO, trVBO, trIBO, trNBO;
	unsigned int wireVAO, wireVBO, wireIBO;

	std::vector<p3> trPositions, trNormals, wirePositions;
	std::vector<unsigned int> trIndices, wireIndices;
	void genBuffers()
	{
		glGenVertexArrays(1, &trVAO);
		glBindVertexArray(trVAO);

		glGenBuffers(1, &trVBO);
		glGenBuffers(1, &trIBO);
		glGenBuffers(1, &trNBO);

		glBindBuffer(GL_ARRAY_BUFFER, trVBO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, trNBO);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);



		glGenVertexArrays(1, &wireVAO);
		glBindVertexArray(wireVAO);

		glGenBuffers(1, &wireVBO);
		glGenBuffers(1, &wireIBO);

		glBindBuffer(GL_ARRAY_BUFFER, wireVBO);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
	}

	void clear()
	{
		trPositions.clear();
		trIndices.clear();
		trNormals.clear();

		wirePositions.clear();
		wireIndices.clear();
	}

	~ShapeRenderer()
	{
		glDeleteVertexArrays(1, &trVAO);
		glDeleteBuffers(1, &trVBO);
		glDeleteBuffers(1, &trIBO);
		glDeleteBuffers(1, &trNBO);

		glDeleteVertexArrays(1, &wireVAO);
		glDeleteBuffers(1, &wireVBO);
		glDeleteBuffers(1, &wireIBO);
	}


	//only at the start and after the data is modified
	//I am assuming the data does not usually change so I am not using subdata
	void updateBuffers()
	{
		glBindVertexArray(trVAO);

		glBindBuffer(GL_ARRAY_BUFFER, trVBO);
		glBufferData(GL_ARRAY_BUFFER, trPositions.size() * sizeof(p3), trPositions.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, trIndices.size() * sizeof(unsigned int), trIndices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, trNBO);
		glBufferData(GL_ARRAY_BUFFER, trNormals.size() * sizeof(p3), trNormals.data(), GL_STATIC_DRAW);
	}



	void draw() const
	{
		glBindVertexArray(trVAO);
		glDrawElements(GL_TRIANGLES, trIndices.size(), GL_UNSIGNED_INT, nullptr);


	}
};


