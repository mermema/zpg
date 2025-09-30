#pragma once
#pragma once
using namespace std;

const char* vertexShader = R"( 
    #version 330 core
    layout(location = 0) in vec3 pos;
    void main() {
        gl_Position = vec4(pos, 1.0);
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
