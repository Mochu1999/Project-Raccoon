#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>






struct Text {		//lacks destructor!
	unsigned int textID;


	unsigned int vertexArray, vertexBuffer, ibo;

	vector<float> positions;	//quads
	vector<unsigned int> indices;

	string textToDraw;


	FT_Library ft;
	FT_Face face;
	string glyphPath = R"(C:\dev\C++ libs\Helvetica\Helvetica.otf)";

	string allGlyphs = "abcp .,;?1234567890";



	struct GlyphMetrics {
		float width, height;
		float bearingX, bearingY;
		float advertexArraynce;
		float texCoordX0 = 0, texCoordY0 = 0;
		float texCoordX1 = 1, texCoordY1 = 1;
	};
	std::map<char, GlyphMetrics> glyphMetricsMap;


	int startX, startY;
	Text(string textToDraw_, int startX_, int startY_) :textToDraw(textToDraw_), startX(startX_), startY(startY_) {
		//allGlyphs = textToDraw_;
		initializeFreeType(glyphPath);
		fillTextureAtlas();
		renderGlyph();

		initializeBuffer();
		initializeIBO();
	}


	float widthAtlas = 0;
	float heightAtlas = 0;
	float totalAdvertexArraynce = 0;
	void fillTextureAtlas() {

		float currentX = 0;
		for (char& c : allGlyphs) {

			storeGlyph(c, widthAtlas, totalAdvertexArraynce); //store metrics and increases widthAtlas and totalAdvertexArraynce
			if (glyphMetricsMap[c].height > heightAtlas) {
				heightAtlas = glyphMetricsMap[c].height;
			}
		}

		createTextureAtlas(); //sets empty texture

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //Sets the unpack alignment to 1 byte to handle bitmaps that are not aligned to 4 bytes

		for (char& c : allGlyphs) { //stores each metric in the texture

			glBindTexture(GL_TEXTURE_2D, textID);//necessary here

			if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
				std::cerr << "Failed to load glyph " << c << std::endl;
				continue;
			}
			FT_Bitmap& bitmap = face->glyph->bitmap;





			//cout << "widthAtlas: " << widthAtlas << " heightAtlas: " << heightAtlas << endl;
			//cout << "face->glyph->bitmap.width: " << face->glyph->bitmap.width << " face->glyph->bitmap.rows: " << face->glyph->bitmap.rows << endl;
			//cout << "currentX: " << currentX << endl;

			glTexSubImage2D(
				GL_TEXTURE_2D,
				0, //detail number, 0 is base image level
				currentX, // x position in the texture from the left
				0, // y position in the texture
				bitmap.width, // width of the subimage
				bitmap.rows, //  height of the subimage
				GL_RED,
				GL_UNSIGNED_BYTE,
				bitmap.buffer // The actual bitmap data for the glyph.
			);



			float yOffset = 0;

			glyphMetricsMap[c].texCoordX0 = currentX / widthAtlas;
			glyphMetricsMap[c].texCoordY0 = yOffset / heightAtlas;
			glyphMetricsMap[c].texCoordX1 = (currentX + glyphMetricsMap[c].width) / widthAtlas;
			glyphMetricsMap[c].texCoordY1 = (yOffset + glyphMetricsMap[c].height) / heightAtlas;

			currentX += glyphMetricsMap[c].width;


		}
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		/*for (auto& pair : glyphMetricsMap) {
			cout << endl << endl;
			char i = pair.first;
			cout << "Glyph: " << i << endl;
			cout << "glyphMetricsMap[i].width: " << glyphMetricsMap[i].width << endl;
			cout << "glyphMetricsMap[i].height: " << glyphMetricsMap[i].height << endl;
			cout << "glyphMetricsMap[i].bearingX: " << glyphMetricsMap[i].bearingX << endl;
			cout << "glyphMetricsMap[i].bearingY: " << glyphMetricsMap[i].bearingY << endl;
			cout << "glyphMetricsMap[i].advertexArraynce: " << glyphMetricsMap[i].advertexArraynce << endl;
			cout << "glyphMetricsMap[i].texCoordX0: " << glyphMetricsMap[i].texCoordX0 << endl;
			cout << "glyphMetricsMap[i].texCoordY0: " << glyphMetricsMap[i].texCoordY0 << endl;
			cout << "glyphMetricsMap[i].texCoordX1: " << glyphMetricsMap[i].texCoordX1 << endl;
			cout << "glyphMetricsMap[i].texCoordY1: " << glyphMetricsMap[i].texCoordY1 << endl << endl;
		}*/

		glBindTexture(GL_TEXTURE_2D, 0);
	}



	void storeGlyph(char character, float& widthAtlas, float& totalAdvertexArraynce) {		//Stores metrics inside a map //change to allow multiple glyphs


		if (FT_Load_Char(face, character, FT_LOAD_RENDER)) {
			std::cerr << "Failed to load glyph" << std::endl;

		}



		FT_Bitmap& bitmap = face->glyph->bitmap;

		float bearingX = face->glyph->bitmap_left;
		float bearingY = face->glyph->bitmap_top;
		float w = bitmap.width;
		float h = bitmap.rows;
		/*float offsetX = bearingX;
		float offsetY = (bearingY - bitmap.rows);*/
		float advertexArraynce = face->glyph->advance.x >> 6;


		widthAtlas += w;
		totalAdvertexArraynce += advertexArraynce;


		glyphMetricsMap.emplace(character, GlyphMetrics{ w, h, bearingX, bearingY, advertexArraynce });

	}

	void pushIndices(size_t i) {
		unsigned int aux = i * 4;
		indices.insert(indices.end(), { aux,aux + 1,aux + 2,aux,aux + 2,aux + 3 });
	}

	void renderGlyph() {	//creates the quads and takes positions from the texture

		int x = startX;
		int y = startY;

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


			x += metrics.advertexArraynce;

			pushIndices(i);
		}

	}


	void createTextureAtlas() {
		glGenTextures(1, &textID);
		glBindTexture(GL_TEXTURE_2D, textID);



		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,	//this gives the internal format, look for more info
			widthAtlas, // width of the entire texture atlas.
			heightAtlas, //height of the tallest glyph in the atlas, used as the atlas height.
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			nullptr
		);



		/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/ //smoother, but somewhat blurry 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //pixelated, but sharp 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_2D, 0);

	}




	void initializeFreeType(const std::string& fontPath) {
		if (FT_Init_FreeType(&ft)) {
			std::cerr << "Could not init FreeType Library" << std::endl;
			return;
		}

		if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
			std::cerr << "Failed to load font" << std::endl;
			return;
		}

		FT_Set_Pixel_Sizes(face, 0, 48); // Set font size
	}


	void initializeBuffer() {
		glBindVertexArray(0);
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), /*positions.data()*/ nullptr, GL_DYNAMIC_DRAW);



		// Position attribute
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

		// text coordinate attribute
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		//glBindVertexArray(0);
	}

	void initializeIBO() {


		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * 4, indices.data(), GL_DYNAMIC_DRAW);
	}

	void draw() {
		glBindVertexArray(0);
		glBindVertexArray(vertexArray);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBindTexture(GL_TEXTURE_2D, textID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, positions.size() * 4, positions.data());
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
	}

	void textUnbind() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	~Text() {
		// Clean up the resources
		glDeleteTextures(1, &textID);
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteBuffers(1, &ibo); // Ensure you've created and stored the IBO handle
		glDeleteVertexArrays(1, &vertexArray);

		// Cleanup FreeType resources
		FT_Done_Face(face);
		FT_Done_FreeType(ft);
	}

};