#pragma once
#include <string>
#include <GL/glew.h>
#include <glm/fwd.hpp>
#include <vector>
#include "Shader.h"
#include "Observer.h"
#include "Camera.h"
using namespace std;

class ShaderProgram : public Observer {
public:
    ShaderProgram(vector<std::shared_ptr<Shader>>& shaders);
    ShaderProgram(vector<Shader*>& shaders);
    ~ShaderProgram();

    bool load(const string& vertexSrc, const string& fragmentSrc);

    void use() const;

    GLuint id() const { return programId; }

    void set(const string& name, float value) const;
    void set(const string& name, int value) const;
    void set(const string& name, const glm::vec3& value) const;
    void set(const string& name, const glm::vec4& value) const;
    void set(const string& name, const glm::mat4& value) const;
    void update(Observable* who) override;
private:
    GLuint programId;
};
