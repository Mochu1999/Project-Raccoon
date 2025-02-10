#pragma once

#include "Common.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>



//////////////////////////// 
/*
General pipeline:
The objective is to construct the class with the font name and the size at the start.
It automatically creates a texture atlas looping through storeGlyph in createAtlasTexture, storing the metrics

Considerations:
A lot of variables should be cleared after the createAtlasTexture creation. These variables should be inside functions and not as class variables
Can't have different font types, nor sizes
*/


struct Text {
	unsigned int textAtlasTexture;


	unsigned int vertexArray, vertexBuffer, indexBuffer;

	vector<float> positions;	//quads
	vector<unsigned int> indices;



	FT_Library ft;
	//A typeface (or font family) englobes all the stylings of a font, a typefont is Helvetica while a font is Helvetica Regular
	FT_Face face; //a face is the structure that stores all the data for a specific font

	string glyphPath;
	int fontPixelSize;

	string allGlyphs; //a font is a collection of glyphs


	//initializes the library and loads the font face
	void initializeFreeType(const std::string& fontPath, const int fontPixelSize);


	//in oneNote you have images of what every metric is.
	struct GlyphMetrics {
		float width, height;
		float bearingX, bearingY;
		float advance;
		float texCoordX0 = 0, texCoordY0 = 0;
		float texCoordX1 = 1, texCoordY1 = 1;
	};
	std::map<char, GlyphMetrics> glyphMetricsMap; //will store all the metrics

	struct TextData {

		vector<string> textToDraw;
		vector<p2> textPosition; //positions in the screen where the text will be rendered
	};
	TextData textData;

	
	//You should have a main constructor to build the atlas and choose the font and functions to add text to be all rendered in one call
	Text(string glyphPath_, int fontPixelSize_) :glyphPath(glyphPath_), fontPixelSize(fontPixelSize_){

		initializeFreeType(glyphPath, fontPixelSize);
		initializeBuffer();
		createAtlasTexture();
		

	}
	
	void addText(string textToDraw_, p2 textPosition_) {

		textData.textToDraw.push_back(textToDraw_);
		textData.textPosition.push_back(textPosition_);

		
	};

	void substituteText(int index, string textToDraw_, p2 textPosition_) {
		textData.textToDraw[index] = textToDraw_;
		textData.textPosition[index] = textPosition_;

		renderGlyph();
	}

	//Creates and sets the texture of the atlas first without data
	void initializeAtlasTexture(const float atlasWidth, const float atlasHeight);


	//Stores the metrics of a character inside a map and the sum of its advances gets the width of the atlas
	void storeGlyph(char character, float& atlasWidth);

	//main function that includes the initialization of the texture and the call of storeGlyph to end with the final Atlas
	void createAtlasTexture();
	

	

	//fills the vertex buffer with the final quad positions and the atlas coordinates of the glyph
	void renderGlyph();

	int indexOffset = 0; //to separate different textToDraws
	//2 triangles per each quad in the order {0,1,2 , 0,2,3}
	void createIndices(size_t i);



	void initializeBuffer();

	//static draw
	void sDraw() {
		glBindVertexArray(0);
		glBindVertexArray(vertexArray);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), /*positions.data()*/ nullptr, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * 4, indices.data(), GL_DYNAMIC_DRAW);

		glBindTexture(GL_TEXTURE_2D, textAtlasTexture);
		glBufferSubData(GL_ARRAY_BUFFER, 0, positions.size() * 4, positions.data());
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
	}


	~Text();

};