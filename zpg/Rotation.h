// Rotation.h
#pragma once
#include "BasicTransformation.h"
#include <glm/gtc/matrix_transform.hpp>

class Rotation : public BasicTransformation {
public:
    Rotation(const glm::vec3& axis) : axis(axis) {}
    Rotation(const glm::vec3& axis, float time)
        : axis(axis), modelMatrix(glm::rotate(glm::mat4(1.0f), time, axis)) {
    }

    void update(float time) override {
        modelMatrix = glm::rotate(glm::mat4(1.0f), time, axis);
    }


    glm::mat4 getMatrix() const override {
        return modelMatrix;
    }
private:
    glm::vec3 axis;
    glm::mat4 modelMatrix;
};

