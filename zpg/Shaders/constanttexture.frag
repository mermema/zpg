#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform bool hasTexture;
uniform sampler2D diffuseMap;
uniform vec3 objectColor;

void main()
{
    vec3 color;

    if (hasTexture) {
        color = texture(diffuseMap, TexCoords).rgb;
    } else {
        color = objectColor;
    }

    FragColor = vec4(color, 1.0);
}
