#pragma once
#include "Light.h"
#include "ShaderProgram.h"


class DirectionalLight : public Light {
private:
    glm::vec3 direction;

public:
    DirectionalLight(const glm::vec3& dir = glm::vec3(0.0f, -1.0f, 0.0f),
        const glm::vec3& col = glm::vec3(1.0f),
        float intens = 0.5f)
        : Light(glm::vec3(0.0f), col, intens), direction(glm::normalize(dir)) {
        setType(2); // directional
        setAttenuation(1.0f, 0.0f, 0.0f);
    }

    void setDirection(const glm::vec3& newDir) {
        direction = glm::normalize(newDir);
        notifyObservers();
    }

    glm::vec3 getDirection() const { return direction; }

    void applyToShader(ShaderProgram* shader) const override {
        Light::applyToShader(shader); 
        shader->use();
        shader->setLightDirection(getIndex(), direction);
        shader->unset();
    }
};