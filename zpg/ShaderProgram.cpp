#include "ShaderProgram.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "Scene.h"

ShaderProgram::ShaderProgram(vector<Shader*>& shaders) {
    programId = glCreateProgram();

    for (auto shader : shaders) {
        shader->attachShader(programId);
    }

    glLinkProgram(programId);

    GLint success;
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
        char log[2048];
        glGetProgramInfoLog(programId, sizeof(log), nullptr, log);
        std::cerr << "Program link error:\n" << log << std::endl;
    }
}
void ShaderProgram::use() const {
    glUseProgram(programId);
}

void ShaderProgram::unset() const {
    glUseProgram(0);
}

ShaderProgram::~ShaderProgram() {
    if (programId) glDeleteProgram(programId);
}

void ShaderProgram::set(const string& name, float value) const {
    GLint loc = glGetUniformLocation(programId, name.c_str());
    glUniform1f(loc, value);
}

void ShaderProgram::set(const string& name, int value) const {
    GLint loc = glGetUniformLocation(programId, name.c_str());
    glUniform1i(loc, value);
}

void ShaderProgram::set(const string& name, const glm::vec3& value) const {
    GLint loc = glGetUniformLocation(programId, name.c_str());
    glUniform3fv(loc, 1, glm::value_ptr(value));
}

void ShaderProgram::set(const string& name, const glm::vec4& value) const {
    GLint loc = glGetUniformLocation(programId, name.c_str());
    glUniform4fv(loc, 1, glm::value_ptr(value));
}

void ShaderProgram::set(const string& name, const glm::mat4& value) const {
    GLint loc = glGetUniformLocation(programId, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}


void ShaderProgram::setMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
{   
    const std::string& prefix = "material";
    set(prefix + ".ambient", ambient);
    set(prefix + ".diffuse", diffuse);
    set(prefix + ".specular", specular);
    set(prefix + ".shininess", shininess);

}

void ShaderProgram::setSkyDomeTexture(int value)
{
    set("skyTexture", value);
}

void ShaderProgram::setSkybox(int value)
{
    set("skybox", value);
}
void ShaderProgram::setViewMatrix(glm::mat4 matrix)
{
    set("view", matrix);
}
void ShaderProgram::setProjectionMatrix(glm::mat4 matrix)
{
    set("projection", matrix);
}
void ShaderProgram::setModelMatrix(glm::mat4 matrix)
{
    set("model", matrix);
}


void ShaderProgram::setHasTexture(int value)
{
    set("hasTexture", value);
}

void ShaderProgram::setLightUniforms(int lightindex, int type, glm::vec3 color, glm::mat4 lightMatrix, float intensity, float constant, float linear, float quadratic)
{   
    std::string base = "lights[" + std::to_string(lightindex) + "].";

    set(base + "type", type);
    set(base + "color", color); 
    set(base + "lightMatrix", lightMatrix);
    set(base + "intensity", intensity);
    set(base + "constant", constant);
    set(base + "linear", linear);
    set(base + "quadratic", quadratic);

}

void ShaderProgram::setLightAngle(int lightindex, float angle)
{
    std::string base = "lights[" + std::to_string(lightindex) + "].";
    set(base + "angle", angle);
}

void ShaderProgram::setLightDirection(int lightindex, glm::vec3 direction)
{
    std::string base = "lights[" + std::to_string(lightindex) + "].";
    set(base + "direction", direction);
}

void ShaderProgram::setObjectColor(glm::vec3 color)
{
    set("objectColor", color);
}


void ShaderProgram::setMaterialDifuseMap(int textureUnit)
{
    set("material.diffuseMap", textureUnit);
}

void ShaderProgram::setNumberOfLights(int value)
{
    set("numberOfLights", value);
}

void ShaderProgram::CameraViewPos(glm::vec3 eyePos)
{
    set("viewPos", eyePos);
}





void ShaderProgram::update(Observable* who) {
    Camera* camera = dynamic_cast<Camera*>(who);
    if (camera) {
        use();



        camera->applyToShader(this);

        return;
    }

    Light* light = dynamic_cast<Light*>(who);
    if (light) {
        use();
        light->applyToShader(this);
        return;
    }
    unset();
}

