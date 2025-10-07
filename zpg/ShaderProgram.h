#pragma once
#include <string>
#include <GL/glew.h>
#include <glm/fwd.hpp>
#include "Camera.h"
using namespace std;

class ShaderProgram {
public:
    ShaderProgram() : programId(0) {}
    ~ShaderProgram();

    bool load(const string& vertexSrc, const string& fragmentSrc);

    void use() const;

    GLuint id() const { return programId; }

    void setViewProjection(glm::mat4 view, glm::mat4 proj) {
        use();
        GLint locView = glGetUniformLocation(programId, "viewMatrix");
        GLint locProj = glGetUniformLocation(programId, "projectionMatrix");
        glUniformMatrix4fv(locView, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(locProj, 1, GL_FALSE, &proj[0][0]);
    }

    void set(const string& name, float value) const;
    void set(const string& name, int value) const;
    void set(const string& name, const glm::vec3& value) const;
    void set(const string& name, const glm::vec4& value) const;
    void set(const string& name, const glm::mat4& value) const;

private:
    GLuint programId;
    Camera* m_camera;

};
