#pragma once

#include "Common.hpp"

//draw lines defined in batches, these batches creates indices for their components
struct Lines2D {

	GLenum usageHint = GL_DYNAMIC_DRAW; //Pass this into the constructor for the final version

	bool isBufferUpdated = false;


	vector<p2> positions;

	//indices must be given in a flat array bc opengl expects it as so. If I need to use the indices at any moment for any othe reason
	// have it in the ui2 format and have a conversion function to flatten it for the gpu
	vector <unsigned int> indices;

	unsigned int vertexBuffer;
	unsigned int vertexArray;
	unsigned int indexBuffer;

	//Setting initial buffer size
	size_t currentBufferSize = 0;
	size_t currentDataSize = 0;


	Lines2D() {
		genBuffers();
	}

	void clear() {
		positions.clear(); indices.clear(); setOffset = 0;
	}


	void genBuffers() {
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		glGenBuffers(1, &vertexBuffer);
		glGenBuffers(1, &indexBuffer);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
	}



	void draw() {

		glBindVertexArray(vertexArray);

		//flag activates only when an addSet is Called, otherwise the buffer remains the same
		if (isBufferUpdated)
		{
			currentDataSize = positions.size() * sizeof(p2);

			//resizes both buffers. In this specific case both have the same size
			// positions.size() * sizeof(p) == indices.size() * sizeof(unsigned int)
			if (currentDataSize > currentBufferSize)
			{
				//In the future cap this so it is being multiplied by 2 for small buffers but if it is bigger than some number then only by a 
				// smaller factor 
				currentBufferSize = currentDataSize * 2;

				glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
				glBufferData(GL_ARRAY_BUFFER, currentBufferSize, nullptr, usageHint);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, currentBufferSize, nullptr, usageHint);
			}

			//glBufferData creates a bigger buffer than necessary but glBufferSubData sends the exact amount of memory
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			glBufferSubData(GL_ARRAY_BUFFER, 0, currentDataSize, positions.data());

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, currentDataSize, indices.data());


			isBufferUpdated = false;
		}

		glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
	}


	~Lines2D() {
		glDeleteVertexArrays(1, &vertexArray);
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteBuffers(1, &indexBuffer);

		positions.clear(); indices.clear();
	}





	unsigned int setOffset = 0;	//the offset that lets multiple indices be over the past ones

	//It doesn't reset indices each time, items are expected to be full models, can't be a single pair
	void addSet(const vector<p2>& items) {

		positions.clear(); indices.clear();

		positions.reserve(items.size());
		indices.reserve(items.size() * 2);

		positions.insert(positions.end(), items.begin(), items.end());

		for (unsigned int i = 0; i < items.size() - 1; i++)
		{
			indices.emplace_back(i + setOffset);
			indices.emplace_back(i + setOffset + 1);
		}

		setOffset += items.size();

		isBufferUpdated = true;
	}

};