#pragma once

#include "Common.hpp"
#include "Polygons.hpp"

//To have sharp illumination, you need to repeat positions to have different normals per vertex.
//If you don't repeat positions you can only have one normal per vertex (soft illumination)

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

//////////////////
//From Pyramid.hpp, to check if they have any value. Just delete it if not used by september 2025

//vertex data must be unique for each face (repeated positions)
//void calculateNormals1() {
//	normals.clear();
//	normals.resize(positions.size(), { 0,0,0 });
//
//	for (int i = 0; i < indices.size(); i += 3) {
//		vec3<float> pos1 = positions[indices[i]];
//		vec3<float> pos2 = positions[indices[i + 1]];
//		vec3<float> pos3 = positions[indices[i + 2]];
//
//		vec3<float> normal = normalize3(cross3(pos2 - pos1, pos3 - pos1));
//
//		normals[indices[i]] = normal;
//		normals[indices[i + 1]] = normal;
//		normals[indices[i + 2]] = normal;
//	}
//
//	// Normalize the normals
//	for (vec3<float>& normal : normals) {
//		normal = normalize3(normal);
//	}
//}

//normal averaging for shared vertices (unrepeated positions)
//void calculateNormals2() {
//	normals.clear();
//	normals.resize(positions.size(), { 0, 0, 0 });
//
//	// First, compute per-triangle normals and sum them to the corresponding vertex normals
//	for (int i = 0; i < indices.size(); i += 3) {
//		p3 pos1 = positions[indices[i]];
//		p3 pos2 = positions[indices[i + 1]];
//		p3 pos3 = positions[indices[i + 2]];
//
//		p3 normal = normalize3(cross3(pos2 - pos1, pos3 - pos1));
//
//		normals[indices[i]] += normal;
//		normals[indices[i + 1]] += normal;
//		normals[indices[i + 2]] += normal;
//	}
//
//	// Normalize all vertex normals
//	for (p3& normal : normals) {
//		normal = normalize3(normal);
//	}
//}
////not tested, to have sharp edges
//void calculateNormalsWithSharpEdges(float sharpAngleDegrees) {
//	normals.clear();
//	normals.resize(positions.size(), { 0, 0, 0 });
//
//	// Pre-calculate all face normals
//	std::vector<vec3<float>> faceNormals(indices.size() / 3);
//
//	for (int i = 0; i < indices.size(); i += 3) {
//		vec3<float> pos1 = positions[indices[i]];
//		vec3<float> pos2 = positions[indices[i + 1]];
//		vec3<float> pos3 = positions[indices[i + 2]];
//
//		vec3<float> faceNormal = normalize3(cross3(pos2 - pos1, pos3 - pos1));
//		faceNormals[i / 3] = faceNormal;
//	}
//
//	// Accumulate normals for each vertex but only if faces are smooth enough
//	float cosSharpAngleThreshold = cos(sharpAngleDegrees * PI / 180.0f); // Convert degrees to radians
//
//	for (int i = 0; i < indices.size(); i += 3) {
//		int idx1 = indices[i];
//		int idx2 = indices[i + 1];
//		int idx3 = indices[i + 2];
//
//		vec3<float> faceNormal = faceNormals[i / 3];
//
//		// For each edge, compare the normals of adjacent faces
//		addNormalIfSmooth(idx1, idx2, faceNormal, faceNormals, cosSharpAngleThreshold);
//		addNormalIfSmooth(idx2, idx3, faceNormal, faceNormals, cosSharpAngleThreshold);
//		addNormalIfSmooth(idx3, idx1, faceNormal, faceNormals, cosSharpAngleThreshold);
//	}
//
//	// Normalize the accumulated normals
//	for (vec3<float>& normal : normals) {
//		normal = normalize3(normal);
//	}
//}
//
//// Helper function to accumulate normal for a vertex if the adjacent face normals are smooth
//void addNormalIfSmooth(int vertexIndex1, int vertexIndex2, vec3<float>& faceNormal,
//	const std::vector<vec3<float>>& faceNormals, float cosThreshold) {
//	for (int i = 0; i < indices.size(); i += 3) {
//		// Check if vertexIndex1 and vertexIndex2 belong to another face
//		if ((indices[i] == vertexIndex1 && indices[i + 1] == vertexIndex2) ||
//			(indices[i] == vertexIndex2 && indices[i + 1] == vertexIndex1) ||
//			(indices[i] == vertexIndex1 && indices[i + 2] == vertexIndex2) ||
//			(indices[i] == vertexIndex2 && indices[i + 2] == vertexIndex1)) {
//
//			vec3<float> adjacentNormal = faceNormals[i / 3];
//			if (dot3(faceNormal, adjacentNormal) >= cosThreshold) {
//				normals[vertexIndex1] += faceNormal;
//				normals[vertexIndex2] += faceNormal;
//			}
//		}
//	}
//}