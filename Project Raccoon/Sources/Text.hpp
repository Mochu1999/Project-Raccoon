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

	string textToDraw;


	FT_Library ft;
	//A typeface (or font family) englobes all the stylings of a font, a typefont is Helvetica while a font is Helvetica Regular
	FT_Face face; //a face is the structure that stores all the data for a specific font
	string glyphPath = "resources/Glyphs/Helvetica/Helvetica.otf";

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


	int positionX, positionY; //positions in the screen where the text will be rendered
	//You should have a main constructor to build the atlas and choose the font and functions to add text to be all rendered in one call
	Text(string textToDraw_, int positionX_, int positionY_) :textToDraw(textToDraw_), positionX(positionX_), positionY(positionY_) {

		initializeFreeType(glyphPath, 48);
		createAtlasTexture();
		renderGlyph();

		initializeBuffer();
	}

	//Creates and sets the texture of the atlas first without data
	void initializeAtlasTexture(const float atlasWidth, const float atlasHeight) {
		glGenTextures(1, &textAtlasTexture);
		glBindTexture(GL_TEXTURE_2D, textAtlasTexture);



		glTexImage2D(
			GL_TEXTURE_2D,
			0,//level of detail, setted on base detail
			GL_RED,	//this gives the internal format, look for more info
			atlasWidth, // width of the entire texture atlas.
			atlasHeight, //height of the tallest glyph in the atlas, used as the atlas height.
			0, //border of the texture, must be 0
			GL_RED,
			GL_UNSIGNED_BYTE,
			nullptr);



		/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/ //linearly interpolates the pixel, smoother, but somewhat blurry 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //choose the nearest pixel: pixelated, but sharp 
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//clamps texture outside 0,1 range? Don't think I need it

		glBindTexture(GL_TEXTURE_2D, 0);
	}

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

		float currentAdvance = 0; //sets the x position of the glyph in the atlas
		for (char& c : allGlyphs) { //stores a texture of each glyph in the atlas

			glBindTexture(GL_TEXTURE_2D, textAtlasTexture);//necessary here

			if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
				std::cout << "Failed to load glyph " << c << std::endl;
				continue;
			}


			//cout << "atlasWidth: " << atlasWidth << " atlasHeight: " << atlasHeight << endl;
			//cout << "face->glyph->bitmap.width: " << face->glyph->bitmap.width << " face->glyph->bitmap.rows: " << face->glyph->bitmap.rows << endl;
			//cout << "currentAdvance: " << currentAdvance << endl;

			FT_Bitmap& bitmap = face->glyph->bitmap; //A bitmap is the FreeType structure that contains the data of a glyph
			glTexSubImage2D(
				GL_TEXTURE_2D,
				0, //level of detail, setted on base detail
				currentAdvance, //X offset in the texture where the subimage will be placed
				0, //Y offset in the texture
				bitmap.width, // width of the subimage (glyph)
				bitmap.rows, //  height of the subimage
				GL_RED,
				GL_UNSIGNED_BYTE,
				bitmap.buffer // The actual bitmap data for the glyph.
			);












			////////////////////////////////////////////////////////////////////////////////////////////////////////// Falta por entender

			float yOffset = 0;

			glyphMetricsMap[c].texCoordX0 = currentAdvance / atlasWidth;
			glyphMetricsMap[c].texCoordY0 = yOffset / atlasHeight;
			glyphMetricsMap[c].texCoordX1 = (currentAdvance + glyphMetricsMap[c].width) / atlasWidth;
			glyphMetricsMap[c].texCoordY1 = (yOffset + glyphMetricsMap[c].height) / atlasHeight;

			currentAdvance += glyphMetricsMap[c].width;


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


	

	

	void pushIndices(size_t i) {
		unsigned int aux = i * 4;
		indices.insert(indices.end(), { aux,aux + 1,aux + 2,aux,aux + 2,aux + 3 });
	}

	//creates the quads and takes positions from the texture
	void renderGlyph() {	

		int x = positionX;
		int y = positionY;

		for (size_t i = 0; i < textToDraw.length(); ++i) {
			char c = textToDraw[i];



			GlyphMetrics metrics = glyphMetricsMap[c];


			float x0 = x + metrics.bearingX;
			float y0 = y - (metrics.height - metrics.bearingY);
			float x1 = x0 + metrics.width;
			float y1 = y0 + metrics.height;


			float s0 = metrics.texCoordX0;
			float t0 = metrics.texCoordY0;
			float s1 = metrics.texCoordX1;
			float t1 = metrics.texCoordY1;


			positions.insert(positions.end(), {
				x0, y0, s0, t1,
				x1, y0, s1, t1,
				x1, y1, s1, t0,
				x0, y1, s0, t0
				});


			x += metrics.advance;

			pushIndices(i);
		}

	}









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