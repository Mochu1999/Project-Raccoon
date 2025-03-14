#pragma once

#include "Common.hpp"
#include "Polygons.hpp"

struct Polyhedra {

	GLenum usageHint = GL_STATIC_DRAW;


	bool isBufferUpdated = true;


	unsigned int vertexArray;
	unsigned int vertexBuffer;
	unsigned int indexBuffer;
	unsigned int normalsBuffer;

	//Setting initial buffers size
	size_t currentPositionsBufferSize = 0;
	size_t currentIndicesBufferSize = 0;
	size_t currentNormalsBufferSize = 0;

	size_t currentPositionsDataSize = 0;
	size_t currentIndicesDataSize = 0;
	size_t currentNormalsDataSize = 0;

	vector<p3> positions;
	vector<unsigned int> indices;
	vector<p3> normals;

	Polyhedra() {
		genBuffers();
	}

	void addSet() {

	}

	void calculateNormals() {

	}

	void simpleIndices() {
		indices.clear();
		for (int i = 0; i < positions.size(); i++)
		{
			indices.push_back(i);
		}
	}


	void draw() {

		glBindVertexArray(vertexArray);

		/*if (isBufferUpdated)
		{
			currentPositionsDataSize = positions.size() * sizeof(p3);
			currentIndicesDataSize = indices.size() * sizeof(unsigned int);

			if (currentPositionsDataSize > currentPositionsBufferSize || currentIndicesDataSize > currentIndicesBufferSize)
			{
				currentPositionsBufferSize = currentPositionsDataSize * 2;
				currentIndicesBufferSize = currentIndicesDataSize * 2;

				glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
				glBufferData(GL_ARRAY_BUFFER, currentPositionsBufferSize, nullptr, usageHint);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, currentIndicesBufferSize, nullptr, usageHint);

				glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
				glBufferData(GL_ARRAY_BUFFER, currentPositionsBufferSize, nullptr, usageHint);

			}

			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			glBufferSubData(GL_ARRAY_BUFFER, 0, currentPositionsDataSize, positions.data());

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, currentIndicesDataSize, indices.data());

			glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
			glBufferSubData(GL_ARRAY_BUFFER, 0, currentPositionsDataSize, normals.data());

			isBufferUpdated = false;
		}*/
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(p3) , positions.data(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * 4, indices.data(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(p3), normals.data(), GL_DYNAMIC_DRAW);


		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

		/*
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(p3), positions.data(), GL_DYNAMIC_DRAW);

		//glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(p3), normals.data(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * 4, indices.data(), GL_DYNAMIC_DRAW);
		*/
	}



	void genBuffers() {
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		glGenBuffers(1, &vertexBuffer);
		glGenBuffers(1, &indexBuffer);
		glGenBuffers(1, &normalsBuffer);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
	}
	//void genBuffers() {
	//	glGenVertexArrays(1, &vertexArray);
	//	glBindVertexArray(vertexArray);

	//	glGenBuffers(1, &vertexBuffer);
	//	glGenBuffers(1, &indexBuffer);
	//	//glGenBuffers(1, &normalsBuffer);

	//	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	//	glEnableVertexAttribArray(0);
	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	//	//glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
	//	glEnableVertexAttribArray(1);
	//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));

	//	glBindVertexArray(0);
	//}
	
	void clear() {
		positions.clear();
		indices.clear();
		normals.clear();
	}

	~Polyhedra() {
		glDeleteVertexArrays(1, &vertexArray);
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteBuffers(1, &indexBuffer);
	}
};