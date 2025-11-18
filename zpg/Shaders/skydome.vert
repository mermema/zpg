#version 330 core

layout (location = 0) in vec3 vp;
layout (location = 1) in vec3 vn;
layout (location = 2) in vec2 vt;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(view * vec4(vp, 1.0));
    Normal = mat3(transpose(inverse(view))) * vn;
    TexCoords = vt;
    
    gl_Position = projection * vec4(FragPos, 1.0);
}