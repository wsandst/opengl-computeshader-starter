#version 410 core

in vec3 outNormal;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord);
    //FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
}
