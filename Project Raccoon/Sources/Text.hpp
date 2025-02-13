#pragma once

#include "Common.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H



//////////////////////////// 
/*
General pipeline:
The objective is to construct the class with the font name and the size at the start.
It automatically creates a texture atlas looping through storeGlyph in createAtlasTexture, storing the metrics

Considerations:
A lot of variables should be cleared after the createAtlasTexture creation. These variables should be inside functions and not as class variables
It only allows one font type and size per atlas, which I don't know if it positive or negative
No kerning, it can improve spacing between glyphs for specific pairs, like AA vs AV

*/



struct Text {

	unsigned int vertexArray, vertexBuffer, indexBuffer, textAtlasTexture;

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

	
	
	Text(string glyphPath_, int fontPixelSize_) :glyphPath(glyphPath_), fontPixelSize(fontPixelSize_){

		initializeFreeType(glyphPath, fontPixelSize);
		initializeBuffer();
		createAtlasTexture();
		

	}
	
	/*void addText(string textToDraw_, p2 textPosition_) {

		textData.textToDraw.push_back(textToDraw_);
		textData.textPosition.push_back(textPosition_);

		
	};*/

	//If performance becomes an issue working only with concatenated strings is an option
	template <typename... Args> //Variadic arguments
	void addText(p2 textPosition_, Args&&... args)
	{
		// We'll build the final string from all the variadic arguments.
		std::ostringstream oss;
		// Fold expression: essentially does (oss << args) for each argument.
		(oss << ... << args);
		textData.textToDraw.push_back(oss.str());

		textData.textPosition.push_back(textPosition_);
	}

	/*void substituteText(int index, string textToDraw_, p2 textPosition_) {
		textData.textToDraw[index] = textToDraw_;
		textData.textPosition[index] = textPosition_;

		fillVertexBuffer();
	}*/
	template <typename... Args> //Variadic arguments
	void substituteText(int index,p2 textPosition_, Args&&... args)
	{
		// We'll build the final string from all the variadic arguments.
		std::ostringstream oss;
		// Fold expression: essentially does (oss << args) for each argument.
		(oss << ... << args);

		textData.textToDraw[index] = oss.str();
		textData.textPosition[index] = textPosition_;

		fillVertexBuffer();
	}

	//Creates and sets the texture of the atlas first without data
	void initializeAtlasTexture(const float atlasWidth, const float atlasHeight);


	//Stores the metrics of a character inside a map and the sum of its advances gets the width of the atlas
	void storeGlyph(char character, float& atlasWidth);

	//main function that includes the initialization of the texture and the call of storeGlyph to end with the final Atlas
	void createAtlasTexture();
	

	

	//fills the vertex buffer with the final quad positions and with the coordinates of the glyph in the atlas
	void fillVertexBuffer();

	int indexOffset = 0; //to separate different textToDraws
	//2 triangles per each quad in the order {0,1,2 , 0,2,3}
	void createIndices(size_t i);



	void initializeBuffer();

	//static draw //NOT WORKING///////////////////////
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

template<typename T>
T round2d(T number) {
	return round(number * 100.0) / 100.0;
}

template<typename T>
T round1d(T number) {
	return round(number * 10.0) / 10.0;
}