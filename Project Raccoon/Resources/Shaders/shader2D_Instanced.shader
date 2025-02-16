#shader vertex
#version 330 core
layout(location = 0) in vec2 basePosition;      // base geometry for one line (e.g., a unit line)
//layout(location = 1) in vec2 instanceOffset;      // per-instance translation offset
layout(location = 1) in vec2 translation;
layout(location = 2) in vec2 rotation;    // (cosTheta, sinTheta)
layout(location = 3) in vec2 scale;

uniform mat4 u_OrthoProjection;

//void main() {
//    vec2 pos = basePosition + instanceOffset;
//    gl_Position = u_OrthoProjection * vec4(pos, 0.0, 1.0);
//}
void main() {
    // First, apply scaling to the base position:
    vec2 scaled = basePosition * scale;

    // Next, apply rotation:
    // Standard 2D rotation: (x*cos - y*sin, x*sin + y*cos)
    vec2 rotated = vec2(scaled.x * rotation.x - scaled.y * rotation.y,
                        scaled.x * rotation.y + scaled.y * rotation.x);

    // Finally, apply translation:
    vec2 pos = rotated + translation;

    gl_Position = u_OrthoProjection * vec4(pos, 0.0, 1.0);
}




#shader fragment
#version 330 core

uniform vec4 u_Color;

out vec4 fragColor;

void main()
{
	fragColor = u_Color;
}



