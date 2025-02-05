#include "Text.hpp"

//initializes the library and loads the font face
void Text::initializeFreeType(const std::string& fontPath) {
	if (FT_Init_FreeType(&ft)) {
		std::cerr << "Could not init FreeType Library" << std::endl;
		return;
	}

	if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
		std::cerr << "Failed to load font" << std::endl;
		return;
	}

	FT_Set_Pixel_Sizes(face, 0, 48); // Set font size 

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

Text::~Text() {
	// Clean up the resources
	glDeleteTextures(1, &textID);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer); // Ensure you've created and stored the indexBuffer handle
	glDeleteVertexArrays(1, &vertexArray);

	// Cleanup FreeType resources
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}