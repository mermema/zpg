#version 330 core

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vn;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float w = 500;

out vec3 FragPos;
out vec3 Normal;


void main() {
    vec4 projectiveSpace = vec4(vp, 1)*w;
    vec4 worldPos = model * projectiveSpace;

    FragPos = worldPos.xyz / worldPos.w;
    Normal = mat3(transpose(inverse(model))) * vn;

    gl_Position = projection * view * vec4(FragPos, 1.0);
}