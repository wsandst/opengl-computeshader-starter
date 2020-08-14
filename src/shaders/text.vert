#version 330 core
layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inTex;
layout (location = 2) in vec3 inColor;

out vec2 texCoords;
out vec3 textColor;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(inPos, 0.0, 1.0);
    texCoords = inTex;
    textColor = inColor;
}  