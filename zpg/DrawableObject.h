#pragma once
#include "Model.h"
#include "ShaderProgram.h"
#include "CompositeTransformation.h"

class DrawableObject {
public:
    DrawableObject(Model* model, ShaderProgram* shader);

    void setTransformation(CompositeTransformation* t);
    CompositeTransformation* getTransformation() const;
    ShaderProgram* getShader() const { return shader; }
    void update(float time);
    void draw() const;

private:
    Model* model;
    ShaderProgram* shader;
    CompositeTransformation* transform;
};
