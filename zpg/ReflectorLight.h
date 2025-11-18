#pragma once
#include "Light.h"

class ReflectorLight : public Light {
protected:
    glm::vec3 direction;
    float angle;         // VNÌJŠÍ úhel ve stupních

public:
    ReflectorLight(const glm::vec3& pos = glm::vec3(0.0f),
        const glm::vec3& dir = glm::vec3(0.0f, 0.0f, -1.0f),
        const glm::vec3& col = glm::vec3(1.0f),
        float intens = 1.0f,
        float angleDeg = 25.0f)
        : Light(pos, col, intens), direction(glm::normalize(dir)),
        angle(angleDeg) {
        setType(3);
        setAttenuation(1.0f, 0.1f, 0.03f);
    }

    virtual void setDirection(const glm::vec3& newDir) {
        direction = glm::normalize(newDir);
        notifyObservers();
    }

    void setAngle(float angleDeg) {
        angle = angleDeg;
        notifyObservers();
    }

    glm::vec3 getDirection() const { return direction; }
    float getAngle() const { return angle; }

    void applyToShader(ShaderProgram* shader) const override;
};