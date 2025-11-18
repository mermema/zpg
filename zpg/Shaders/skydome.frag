#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D skyTexture;

void main()
{
    FragColor = texture(skyTexture, TexCoords);
}