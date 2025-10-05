#pragma once
#include <string>
#include <GL/glew.h>
#include <glm/fwd.hpp>
using namespace std;

class ShaderProgram {
public:
    ShaderProgram() : programId(0) {}
    ~ShaderProgram();

    bool load(const string& vertexSrc, const string& fragmentSrc);

    void use() const;

    GLuint id() const { return programId; }

    void set(const string& name, float value) const;
    void set(const string& name, int value) const;
    void set(const string& name, const glm::vec3& value) const;
    void set(const string& name, const glm::vec4& value) const;
    void set(const string& name, const glm::mat4& value) const;

private:
    GLuint programId;
};
