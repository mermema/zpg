#pragma once
#pragma once
using namespace std;

const char* vertexShaderCam = R"(
#version 330 core

layout(location = 0) in vec3 vp;   // pozice vrcholu
layout(location = 1) in vec3 vn;   // normála vrcholu

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragColor;  // ZMĚNA: vertexColor -> fragColor

void main() {
    fragColor = vn;
    gl_Position = projection * view * model * vec4(vp, 1.0);
}
)";


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

const char* fragmentShaderBrown = R"(
#version 330 core

in vec3 fragColor;
out vec4 FragColor;

void main() {
    FragColor = vec4(0.7, 0.7, 0.7, 1.0);
}
)";


const char* vertexShaderPhong = R"(
#version 330 core

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vn;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;


void main() {
    FragPos = vec3(model * vec4(vp, 1.0));
    Normal = vn;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
)";


const char* fragmentShaderPhong = R"(
#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

// SVĚTLO
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main() {
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    
    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
    
    // Barva - dočasně pevná
    vec3 objectColor = norm;//vec3(0.8, 0.5, 0.2);
    
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
)";

/*

const char* vertexShaderPhong = R"(
#version 330 core

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vn;
layout(location = 2) in vec3 color;  // ⬅️ PŘIDEJ BARVU Z MODELU!

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec3 VertexColor;  // ⬅️ POŠLI BARVU DO FRAGMENT SHADERU

void main() {
    FragPos = vec3(model * vec4(vp, 1.0));
    Normal = vn;
    VertexColor = color;  // ⬅️ PŘEDEJ BARVU!
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
)";  
const char* fragmentShaderPhong = R"(
#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec3 VertexColor;  // ⬅️ PŘIJMI BARVU Z VERTEX SHADERU

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main() {
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // ⚠️ POUŽIJ VertexColor MÍSTO objectColor!
    vec3 result = (ambient + diffuse + specular) * VertexColor;
    FragColor = vec4(result, 1.0);
}
)";


*/