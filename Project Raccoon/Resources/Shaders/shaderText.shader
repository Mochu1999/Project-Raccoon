#shader vertex 
#version 330 core

//EL ERROR POR LEADING SPACE

layout(location = 0) in vec4 textPos; //POR QUE CUANDO CREAS UN VAO NO TIENES QUE ESTAR BINDED?
layout(location = 1) in vec2 texCoord; //NOMBRE DE MIERDA


uniform mat4 u_OrthoProjection;

out vec2 v_TexCoord; 


void main() {
		gl_Position = u_OrthoProjection * textPos;
		v_TexCoord = texCoord;	
    
}



//LETRAS TRANSPARENTES Y COLORES, CAMBIAR PIPELINE EN GENERAL
#shader fragment
#version 330 core


uniform sampler2D u_Texture; //LITERALMENTE NO ESTÁ HACIENDO NADA, NO EXISTE EN MAIN
in vec2 v_TexCoord;

out vec4 FragColor;

void main()
{

    vec4 textColor = texture(u_Texture, v_TexCoord); // Sample the glyph texture
    vec4 backgroundColor = vec4(0.035f, 0.065f, 0.085f, 1.0f); // Hardcoded background color

    // Blend the text color (alpha determines visibility of glyph) with the background color
    FragColor = mix(backgroundColor, vec4(textColor.rgb, 1.0), textColor.r); 

	//FragColor = texture(u_Texture, v_TexCoord);

}



