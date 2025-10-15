#pragma once
#include <string>
#include <GL/glew.h>
#include <iostream>

class Shader {
public:
    Shader(const std::string& source, GLenum type);

    void attachShader(GLuint programID);

    ~Shader();

private:
    GLuint shaderID;
};
