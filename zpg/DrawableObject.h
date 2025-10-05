#pragma once
#include "Model.h"
#include "ShaderProgram.h"
#include "Transformation.h"

class DrawableObject {
public:
    DrawableObject(Model* model, ShaderProgram* shader);

    void setTransformation(Transformation* t);
    Transformation* getTransformation() const;

    void update(float time);
    void draw() const;

private:
    Model* model;
    ShaderProgram* shader;
    Transformation* transform;
};
