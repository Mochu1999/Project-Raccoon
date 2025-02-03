#shader vertex 
#version 330 core

// Assigns the slots in the Vertex Attribute Array where these attributes (position and normal) go
layout(location = 0) in vec3 pos3D; 
layout(location = 1) in vec3 aNormal;


//uniforms are universal variables that can be accessed from other shaders and in main without the use of a VAO
uniform mat4 u_Model;
uniform mat4 u_Perspective;
uniform mat4 u_View;


out vec3 Normal;  //Output that goes to the fragment shader
out vec3 crntPos;


void main() {

	crntPos = pos3D;
	Normal = mat3(u_Model) * aNormal;
	gl_Position = u_Perspective * u_View * u_Model * vec4(crntPos,1.0);
}




#shader fragment
#version 330 core



in vec3 Normal;
in vec3 crntPos;

vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);

uniform int u_fragmentMode; //1 for lit, 0 for full color
uniform vec3 camPos;
uniform vec3 u_lightPos;


uniform vec4 u_Color;


out vec4 FragColor;

void main()
{
	if (u_fragmentMode == 0) 
	{
		float ambient = 0.15f;
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

}



