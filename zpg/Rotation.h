// Rotation.h
#pragma once
#include "BasicTransformation.h"
#include <glm/gtc/matrix_transform.hpp>

class Rotation : public BasicTransformation {
public:
    Rotation(float angle , glm::vec3 axes) : axis(axes), modelMatrix(glm::rotate(glm::mat4(1), angle, axes)) {}

    glm::mat4 getMatrix() const override {
        return modelMatrix;
    }
protected:
    glm::vec3 axis;
    glm::mat4 modelMatrix;
};

