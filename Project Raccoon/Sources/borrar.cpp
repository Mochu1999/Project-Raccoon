//////////////////////////// 
/*
General pipeline:
The objective is to construct the class with the font name and the size at the start.
It automatically creates a texture atlas looping through storeGlyph in fillTextureAtlas, storing the metrics

Considerations:
A lot of variables should be cleared after the fillTextureAtlas creation. These variables should be inside functions and not as class variables
Can't have different font types, nor sizes
*/