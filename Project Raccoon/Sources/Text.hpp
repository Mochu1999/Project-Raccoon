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
		//int textSize;
		vector<int> positionX, positionY; //positions in the screen where the text will be rendered
	};
	TextData textData;

	
	//You should have a main constructor to build the atlas and choose the font and functions to add text to be all rendered in one call
	Text(string glyphPath_, int fontPixelSize_) :glyphPath(glyphPath_), fontPixelSize(fontPixelSize_){

		initializeFreeType(glyphPath, fontPixelSize);
		initializeBuffer();
		createAtlasTexture();
		

	}
	//USA P2 PARA POSITIONS LOCO, PA QUE COÑO TENEMOS UNA LIBRERIA DE 2D
	void addText(string textToDraw, int positionX, int positionY) {
		textData.textToDraw .push_back(textToDraw);
		//textData.textSize = textToDraw.size();
		textData.positionX.push_back(positionX);
		textData.positionY.push_back(positionY);

		
	};

	//Creates and sets the texture of the atlas first without data
	void initializeAtlasTexture(const float atlasWidth, const float atlasHeight);


	//Stores the metrics of a character inside a map and the sum of its advances gets the width of the atlas
	void storeGlyph(char character, float& atlasWidth);

	//main function that includes the initialization of the texture and the call of storeGlyph to end with the final Atlas
	void createAtlasTexture() {

		float atlasWidth = 0, atlasHeight = 0;//Total dimensions of the atlas

		for (char& c : allGlyphs)
		{

			storeGlyph(c, atlasWidth); //creates metrics map and sets atlas dimensions
			if (glyphMetricsMap[c].height > atlasHeight)
			{
				atlasHeight = glyphMetricsMap[c].height; //highest glyph determines final atlasHeight
			}
		}

		initializeAtlasTexture(atlasWidth, atlasHeight);

		//OpenGL by default expects the rows of data to be packed in multiples of 4 bytes, it isn't our case, putting it in 1 somehow means 
		// that “Each row is packed with no extra padding—just one byte per pixel, back-to-back.” and it just works without removing efficiency
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		float currentWidth = 0; //sets the x position of the glyph in the atlas
		for (char& c : allGlyphs) { //stores a texture of each glyph in the atlas

			glBindTexture(GL_TEXTURE_2D, textAtlasTexture);//necessary here

			if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
				std::cout << "Failed to load glyph " << c << std::endl;
				continue;
			}


			//cout << "atlasWidth: " << atlasWidth << " atlasHeight: " << atlasHeight << endl;
			//cout << "face->glyph->bitmap.width: " << face->glyph->bitmap.width << " face->glyph->bitmap.rows: " << face->glyph->bitmap.rows << endl;
			//cout << "currentWidth: " << currentWidth << endl;

			FT_Bitmap& bitmap = face->glyph->bitmap; //A bitmap is the FreeType structure that contains the data of a glyph
			glTexSubImage2D(
				GL_TEXTURE_2D,
				0, //level of detail, setted on base detail
				currentWidth, //X offset in the texture where the subimage will be placed
				0, //Y offset in the texture
				bitmap.width, // width of the subimage (glyph)
				bitmap.rows, //  height of the subimage
				GL_RED,
				GL_UNSIGNED_BYTE,
				bitmap.buffer // The actual bitmap data for the glyph.
			);


			//height is the total height, width is the total width of the glyph, advance is the width plus the space till next glyph
			glyphMetricsMap[c].texCoordX0 = currentWidth / atlasWidth;
			glyphMetricsMap[c].texCoordX1 = (currentWidth + glyphMetricsMap[c].width) / atlasWidth; 
			glyphMetricsMap[c].texCoordY0 = 0;
			glyphMetricsMap[c].texCoordY1 = glyphMetricsMap[c].height / atlasHeight; 

			currentWidth += glyphMetricsMap[c].width;


		}
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4); //Returning to default for next operations

		/*for (auto& pair : glyphMetricsMap) {
			
			char i = pair.first;
			cout << "Glyph: " << i << endl;
			print(glyphMetricsMap[i].width);
			print(glyphMetricsMap[i].height);
			print(glyphMetricsMap[i].bearingX);
			print(glyphMetricsMap[i].bearingY);
			print(glyphMetricsMap[i].advance);
			print(glyphMetricsMap[i].texCoordX0);
			print(glyphMetricsMap[i].texCoordY0);
			print(glyphMetricsMap[i].texCoordX1);
			print(glyphMetricsMap[i].texCoordY1);
			cout << endl << endl;
		}*/

		glBindTexture(GL_TEXTURE_2D, 0);

		//glyphMetricsMap.clear(); //Clears that memory bc it's not going to be used anymore
	}


	

	
	int currentIndex = 0; //to separate different textToDraws
	void pushIndices(size_t i) {
		unsigned int aux = i * 4;
		indices.insert(indices.end(), { currentIndex + aux,currentIndex + aux + 1,currentIndex + aux + 2,currentIndex + aux,currentIndex + aux + 2,currentIndex + aux + 3 });
	}

	//fills the vertex buffer with the final quad positions and the atlas coordinates of the glyph
	void renderGlyph();









	void initializeBuffer();

	//MODIFICAR PARA TEXTO DINÁMICO
	void draw() {
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