#pragma once
#include <string>
#include <GL/glew.h>
#include <glm/fwd.hpp>
#include <vector>
#include "Shader.h"
#include "Observer.h"
#include "Camera.h"
#include "light.h"
using namespace std;

class ShaderProgram : public Observer {
public:
    ShaderProgram(vector<std::shared_ptr<Shader>>& shaders);
    ShaderProgram(vector<Shader*>& shaders);
    ~ShaderProgram();

    bool load(const string& vertexSrc, const string& fragmentSrc);

    void use() const;
    void unset() const;


    GLuint id() const { return programId; }

    void set(const string& name, float value) const;
    void set(const string& name, int value) const;
    void set(const string& name, const glm::vec3& value) const;
    void set(const string& name, const glm::vec4& value) const;
    void set(const string& name, const glm::mat4& value) const;
    void setMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shiness);
    void setSkyDomeTexture(int value);
    void setSkybox(int value);
    void setViewMatrix(glm::mat4 matrix);
    void setProjectionMatrix(glm::mat4 matrix);
    void setModelMatrix(glm::mat4 matrix);
    void setHasTexture(int value);
    void setLightUniforms(int lightindex, int type, glm::vec3 color, glm::mat4 lightMatrix, float intensity, float constant, float linear, float quadratic);
    void setLightAngle(int lightindex, float angle);
    void setLightDirection(int lightindex, glm::vec3 direction);
    void setObjectColor(glm::vec3 color);
    void setMaterialDifuseMap(int textureUnit);
    void setNumberOfLights(int value);
    void CameraViewPos(glm::vec3 eyePos);
    void update(Observable* who) override;
private:
    GLuint programId;
};
