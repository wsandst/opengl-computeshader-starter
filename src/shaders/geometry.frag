#version 410 core

in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D ourTexture;

const vec3 lightDir = vec3(1.0, 1.0, 1.0);

void main()
{
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);

    float combined = 0.3 + diff;
    vec3 finalLight = vec3(combined, combined, combined);

    //FragColor = vec4(finalLight, 1.0);
    FragColor = texture(ourTexture, TexCoord) * vec4(finalLight, 1.0);
    //FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
    //FragColor.rgb = Normal;
    //FragColor.a = 1.0f;
}
