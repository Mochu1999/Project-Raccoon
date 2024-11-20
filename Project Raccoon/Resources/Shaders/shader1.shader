#shader vertex 
#version 330 core

// Assigns the slots in the Vertex Attribute Array where these attributes (position and normal) go
layout(location = 0) in vec3 pos3D; 
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 pos2D;

layout(location = 3) in vec4 textPos; 
layout(location = 4) in vec2 texCoord;

//uniforms are universal variables that can be accessed from other shaders and in main without the use of a VAO
uniform mat4 u_MVP;
uniform mat4 u_OrthoProjection;

uniform int u_vertexMode;

out vec3 Normal;  //Output that goes to the fragment shader
out vec3 crntPos;

out vec2 v_TexCoord; 


void main() {
	if (u_vertexMode == 1) //3d
	{
		crntPos = pos3D;
		Normal = aNormal;
		gl_Position = u_MVP * vec4(crntPos,1.0); //gl_Position is a keyword
	}
	else if (u_vertexMode == 0) //2d
	{
        gl_Position = u_OrthoProjection * vec4(pos2D, 0.0, 1.0);
    }
	else if(u_vertexMode == 2) //text
	{
		gl_Position = u_OrthoProjection * textPos;
		v_TexCoord = texCoord;	
	}
    
}




#shader fragment
#version 330 core



in vec3 Normal;
in vec3 crntPos;

vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);

uniform int u_fragmentMode;
uniform vec3 camPos;
uniform vec3 u_lightPos;


uniform vec4 u_Color;

uniform sampler2D u_Texture;
in vec2 v_TexCoord;

out vec4 FragColor;

void main()
{
	if (u_fragmentMode == 0) 
	{
		float ambient = 0.35f;
		float specularLight = 0.7f;

		// diffuse lighting
		vec3 normal = normalize(Normal);
		vec3 lightDirection = normalize(u_lightPos - crntPos);
		float diffuse = max(dot(normal, lightDirection), 0.0f);

		// specular lighting
		vec3 viewDirection = normalize(camPos - crntPos);
		vec3 reflectionDirection = reflect(-lightDirection, normal);
		float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
		float specular = specAmount * specularLight*1.2;

		// outputs final color
		FragColor =  u_Color * lightColor * (diffuse + ambient + specular);
	}
	if (u_fragmentMode == 1) 
    {
        FragColor = u_Color;
    }
	if (u_fragmentMode == 3) //text
    {
    vec4 textColor = texture(u_Texture, v_TexCoord); // Sample the glyph texture
    vec4 backgroundColor = vec4(0.035f, 0.065f, 0.085f, 1.0f); // Hardcoded background color

    // Blend the text color (alpha determines visibility of glyph) with the background color
    FragColor = mix(backgroundColor, vec4(textColor.rgb, 1.0), textColor.r); 

	//FragColor = texture(u_Texture, v_TexCoord);
}
}



