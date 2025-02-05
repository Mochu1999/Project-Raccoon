#shader vertex 
#version 330 core

layout(location = 0) in vec4 positions;
layout(location = 1) in vec2 texCoord;


uniform mat4 u_OrthoProjection;

out vec2 v_TexCoord; 


void main() {
		gl_Position = u_OrthoProjection * positions;
		v_TexCoord = texCoord;	
    
}



//LETRAS TRANSPARENTES Y COLORES, CAMBIAR PIPELINE EN GENERAL
#shader fragment
#version 330 core


uniform sampler2D u_Texture; //No sé lo que hace, pero no se puede quitar. Pinta que va a ser necesario
in vec2 v_TexCoord;

out vec4 FragColor;


void main()         //FUNCIONA PERO CONTINUA EL MISTERIO DE U_TEXTURE Y LAS POLLADAS DE LAS LOCATIONS
{
    // Sample the glyph's alpha from the texture (again, assuming the red channel holds the glyph mask)
    float alpha = texture(u_Texture, v_TexCoord).r;
    
    // Hardcode white as the text color
    vec4 whiteColor = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 backgroundColor = vec4(0.035, 0.065, 0.085, 1.0); // Your background color

    // Blend the background and white based on the glyph's alpha value
    FragColor = mix(backgroundColor, whiteColor, alpha);
}


