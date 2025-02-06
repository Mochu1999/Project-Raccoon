#include "Text.hpp"

//initializes the library and loads the font face
void Text::initializeFreeType(const std::string& fontPath, const int fontPixelSize) {
	if (FT_Init_FreeType(&ft)) {
		std::cerr << "Could not init FreeType Library" << std::endl;
		return;
	}

	if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
		std::cerr << "Failed to load font" << std::endl;
		return;
	}
	// Set font size in pixels. Word and other apps set the size in dots per inch. RESCALING IS NOT AN OPTION YET
	FT_Set_Pixel_Sizes(face, 0, fontPixelSize); 

	FT_ULong  charcode; //unicode codepoint (the number that represents the character)
	FT_UInt   glyphIndex;

	
	charcode = FT_Get_First_Char(face, &glyphIndex); //it's 32, if you force glyphIndex to be 0 things as ñ appear in allGlyphs (but there's no glyph for it, idk)

	while (glyphIndex != 0) {
		//128 first characters are ascii characters. For latin-1 characters (ñ, á, ü) you should have the next 128 or maybe just delete the if
		if (charcode < 256) 
		{
			allGlyphs.push_back(static_cast<char>(charcode));
		}
		
		charcode = FT_Get_Next_Char(face, charcode, &glyphIndex);
	}

	//std::cout << "Extracted glyphs: " << allGlyphs << std::endl;
}


void Text::initializeBuffer() {
	glBindVertexArray(0);
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &indexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);


	//First 2 floats of the vertex buffer are the position and the next 2 the texture coordinates
	//offset is the last element: 2 floats; stride is the next to last element: 4 floats

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	// text coordinate attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindVertexArray(0);
}

//Creates and sets the texture of the atlas first without data
void Text::initializeAtlasTexture(const float atlasWidth, const float atlasHeight) {
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



	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //linearly interpolates the pixel, smoother, but somewhat blurry 
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);*/ //choose the nearest pixel: pixelated, but sharp 
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//clamps texture outside 0,1 range? Don't think I need it

	glBindTexture(GL_TEXTURE_2D, 0);
}

//Stores the metrics of a character inside a map
void Text::storeGlyph(char character, float& widthAtlas) {

	if (FT_Load_Char(face, character, FT_LOAD_RENDER))
	{
		std::cout << "Failed to load glyph" << std::endl;
	}

	FT_GlyphSlot& glyph = face->glyph;
	FT_Bitmap& bitmap = glyph->bitmap; //A bitmap is the FreeType structure that contains the data of a glyph

	//Glyph metrics
	float bearingX = glyph->bitmap_left;
	float bearingY = glyph->bitmap_top;
	float width = bitmap.width;
	float height = bitmap.rows;
	float advance = glyph->advance.x >> 6;


	widthAtlas += width;


	glyphMetricsMap.emplace(character, GlyphMetrics{ width, height, bearingX, bearingY, advance });

}

Text::~Text() {
	// Clean up the resources
	glDeleteTextures(1, &textAtlasTexture);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer); // Ensure you've created and stored the indexBuffer handle
	glDeleteVertexArrays(1, &vertexArray);

	// Cleanup FreeType resources
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

//fills the vertex buffer with the final quad positions and the atlas coordinates of the glyph
void Text::renderGlyph() {

	

	
	for (size_t i = 0; i < textData.textToDraw.size(); i++)
	{

		//bottom left coordinates of the string to render
		int x = textData.positionX[i];
		int y = textData.positionY[i];

		for (size_t j = 0; j < textData.textToDraw[i].size(); ++j) {

			char c = textData.textToDraw[i][j];
			GlyphMetrics metrics = glyphMetricsMap[c];

			//logic is sound
			float x0 = x + metrics.bearingX;
			float y0 = y - metrics.height + metrics.bearingY;
			float x1 = x0 + metrics.width;
			float y1 = y0 + metrics.height;


			float s0 = metrics.texCoordX0;
			float t0 = metrics.texCoordY0;
			float s1 = metrics.texCoordX1;
			float t1 = metrics.texCoordY1;


			positions.insert(positions.end(), //positions and texture coordinates interleaved
				{ x0, y0, s0, t1,
				x1, y0, s1, t1,
				x1, y1, s1, t0,
				x0, y1, s0, t0
				});


			x += metrics.advance;

			pushIndices(j);
		}
			currentIndex = indices.back() + 1;
	}
	//print(indices);
	//indices = { 0,1,2,0,2,3 };
}