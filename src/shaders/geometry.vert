#version 410 core

uniform mat4 matrix;

layout (location = 0) in vec3 inPos; //3 bytes
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;

out vec3 outNormal;
out vec2 TexCoord;

void main()
{
    outNormal = inNormal;
    TexCoord = inTexCoord;

    gl_Position = matrix * vec4(float(inPos.x), float(inPos.y), float(inPos.z), 1.0f);	
    
}