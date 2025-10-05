// Scale.h
#pragma once
#include "BasicTransformation.h"
#include <glm/gtc/matrix_transform.hpp>

class Scale : public BasicTransformation {
public:
    Scale(const glm::vec3& factor) : factor(factor) {}
    void update(float time) override {
        modelMatrix = glm::scale(glm::mat4(1.0f), factor);
    }
    glm::mat4 getMatrix() const override {
        return modelMatrix;
    }
private:
    glm::vec3 factor;
    glm::mat4 modelMatrix;
};