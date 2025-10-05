#pragma once
#pragma once
using namespace std;

const char* vertexShader = R"(
#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 model;

void main() {
    gl_Position = model * vec4(position, 1.0);
}

)";

const char* fragmentShaderRed = R"(
    #version 330 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
)";

const char* fragmentShaderBlue = R"(
    #version 330 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(0.0, 0.0, 1.0, 1.0);
    }
)";


const char* vertexShader6v = R"(
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

out vec3 fragColor;

uniform mat4 model;

void main() {
    gl_Position = model * vec4(position, 1.0);
    fragColor = color;
}
)";
const char* fragmentShader6v = R"(
#version 330 core

in vec3 fragColor;
out vec4 FragColor;

void main() {
    FragColor = vec4(fragColor, 1.0);
}
)";
