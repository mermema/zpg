#pragma once
#include "Model.h"
#include "ShaderProgram.h"
#include "CompositeTransformation.h"
#include "Texture.h"


class DrawableObject {
public:
    DrawableObject(Model* model, ShaderProgram* shader, Texture* texture = nullptr);

    void setTransformation(CompositeTransformation* t);
    CompositeTransformation* getTransformation() const;
    ShaderProgram* getShader() const { return shader; }
    void draw() const;
    void setObjectColor(glm::vec3 color);
    void setTexture(Texture* tex);
    Texture* getTexture() const;
    unsigned int getID() const { return objectID; }
    void setNotMovable() { notMovable = true; }
    bool getNotMovable() { return notMovable; }


private:
    static int nextID; 
    unsigned int objectID;       
    Texture* texture;
    glm::vec3 objectColor;
    Model* model;
    ShaderProgram* shader;
    CompositeTransformation* transform;
    bool notMovable = false;
};
