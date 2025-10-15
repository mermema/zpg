#include "Shader.h"

Shader::Shader(const std::string& source, GLenum type)
{
    shaderID = glCreateShader(type);
    const char* csrc = source.c_str();
    glShaderSource(shaderID, 1, &csrc, nullptr);
    glCompileShader(shaderID);

    GLint success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shaderID, 512, nullptr, log);
        std::cerr << "Shader compile error: " << log << std::endl;
    }
}

void Shader::attachShader(GLuint programID)
{
    glAttachShader(programID, shaderID);
}

Shader::~Shader()
{
    if (shaderID) glDeleteShader(shaderID);
}
