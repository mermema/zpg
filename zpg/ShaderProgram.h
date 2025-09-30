#pragma once
#include <string>
#include <GL/glew.h>
using namespace std;

class ShaderProgram {
public:
    ShaderProgram() : programId(0) {}
    ~ShaderProgram();

    bool load(const std::string& vertexSrc, const std::string& fragmentSrc);

    void use() const;

    GLuint id() const { return programId; }

private:
    GLuint programId;
};
