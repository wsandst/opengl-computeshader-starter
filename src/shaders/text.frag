#version 330 core
in vec2 texCoords;
in vec3 textColor;

out vec4 color;

uniform sampler2D text;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, texCoords).r);
    color = vec4(textColor, 1.0) * sampled;
}  