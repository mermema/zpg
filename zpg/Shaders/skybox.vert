#version 330 core

layout (location = 0) in vec3 vp;

out vec3 TexCoords;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = vp;
    
    // Vypočítáme pozici
    vec4 viewPos = view * vec4(vp, 1.0);
    vec4 clipPos = projection * viewPos;
    
    // Nastavíme Z na maximum
    gl_Position = vec4(clipPos.x, clipPos.y, clipPos.w, clipPos.w);
}