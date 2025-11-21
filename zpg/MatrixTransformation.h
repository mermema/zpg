#pragma once
#include "BasicTransformation.h"
class MatrixTransformation :
    public BasicTransformation
{

public:
    MatrixTransformation(glm::mat4 mat) : modelMatrix(mat) {}

    glm::mat4 getMatrix() const override {
        return modelMatrix;
    }
protected:
    glm::mat4 modelMatrix;
};

