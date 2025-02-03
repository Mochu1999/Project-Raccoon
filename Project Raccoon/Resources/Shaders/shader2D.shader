#shader vertex 
#version 330 core

layout(location = 2) in vec2 pos2D;

uniform mat4 u_OrthoProjection;

void main() {

	gl_Position = u_OrthoProjection * vec4(pos2D, 0.0, 1.0);

}




#shader fragment
#version 330 core

uniform vec4 u_Color;

out vec4 FragColor;

void main()
{
	FragColor = u_Color;
}



