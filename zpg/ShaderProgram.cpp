#include "ShaderProgram.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

static GLuint compileShader(const string& src, GLenum type) {
    GLuint shader = glCreateShader(type);
    const char* csrc = src.c_str();
    glShaderSource(shader, 1, &csrc, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        cerr << "Shader compile error: " << log << endl;
    }
    return shader;
}

bool ShaderProgram::load(const string& vertexSrc, const string& fragmentSrc) {
    GLuint vs = compileShader(vertexSrc, GL_VERTEX_SHADER);
    GLuint fs = compileShader(fragmentSrc, GL_FRAGMENT_SHADER);

    programId = glCreateProgram();
    glAttachShader(programId, vs);
    glAttachShader(programId, fs);
    glLinkProgram(programId);

    GLint success;
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(programId, 512, nullptr, log);
        cerr << "Program link error: " << log << endl;
        return false;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
    return true;
}

void ShaderProgram::use() const {
    glUseProgram(programId);
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