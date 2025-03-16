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

	void addPolyhedra(const std::string& modelPath) {

		clear();

		std::string basePath = "Resources/Simple polyhedra/";
		std::string path = basePath + modelPath;

		std::ifstream inFile(path, std::ios::binary);
		if (inFile)
		{
			size_t size;

			inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
			positions.resize(size);
			inFile.read(reinterpret_cast<char*>(positions.data()), size * sizeof(p3));

			inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
			normals.resize(size);
			inFile.read(reinterpret_cast<char*>(normals.data()), size * sizeof(p3));

			inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
			indices.resize(size);
			inFile.read(reinterpret_cast<char*>(indices.data()), size * sizeof(unsigned int));
		}
		else
		{
			std::cerr << "Error opening file for reading." << std::endl;
		}
		inFile.close();
	}

	void simpleIndices() {
		indices.clear();
		for (int i = 0; i < positions.size(); i++)
		{
			indices.push_back(i);
		}
	}

	//polyhedras that were stl before, with the same .size() for positions, indices and normals
	//  ,but the size of indices in floats while the others in p3
	//indices in order in triangles {1,2,3,4,5,6,7,8,9}
	void stlDraw() {

		glBindVertexArray(vertexArray);

		//the buffer is not expected to be updated at all. Can use data instead of subdata
		if (isBufferUpdated)
		{
			float stlSize = positions.size() * sizeof(p3);

			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, stlSize, positions.data(), GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, stlSize / 3, indices.data(), GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
			glBufferData(GL_ARRAY_BUFFER, stlSize, normals.data(), GL_DYNAMIC_DRAW);

			isBufferUpdated = false;
		}

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

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