#pragma once
#include <glm/glm.hpp>
#include "Observable.h"
#include "CompositeTransformation.h"
#include <string>
class ShaderProgram;

class Light : public Observable {
private:
    CompositeTransformation* transform;  // Vlastní transformace
    glm::vec3 color;
    float intensity;

public:
    Light(const glm::vec3& pos = glm::vec3(0.0f),
        const glm::vec3& col = glm::vec3(1.0f),
        float intens = 1.0f);

    ~Light();

    void setTransform(CompositeTransformation* t);
    CompositeTransformation* getTransform() const { return transform; }

    void setPosition(const glm::vec3& newPos);
    glm::vec3 getPosition() const;

    void setColor(const glm::vec3& newColor);
    void setIntensity(float newIntensity);

    glm::vec3 getColor() const { return color; }
    float getIntensity() const { return intensity; }

    void applyToShader(ShaderProgram* shader, const std::string& uniformName) const;
    void update(float deltaTime);
    void notifyObservers() override;
};