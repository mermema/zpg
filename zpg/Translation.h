#pragma once
#include "BasicTransformation.h"
#include <glm/gtc/matrix_transform.hpp>

class Translation : public BasicTransformation {
public:
    Translation(const glm::vec3& offset) : offset(offset) {}
    void update(float time) override {
        modelMatrix = glm::translate(glm::mat4(1.0f), offset);
    }
    glm::mat4 getMatrix() const override {
        return modelMatrix;
    }
private:
    glm::vec3 offset;
    glm::mat4 modelMatrix;
};