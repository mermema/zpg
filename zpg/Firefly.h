#pragma once
#include "DrawableObject.h"
#include "Light.h"
#include "CompositeTransformation.h"
#include "Scene.h"
#include "DynamicRotation.h"

class Firefly {
private:
    DrawableObject* visual;  
    Light* light;           
    CompositeTransformation* transform; 

public:
    Firefly(Model* sharedModel, ShaderProgram* sharedShader, Light* lightarg, CompositeTransformation* transformationarg, glm::vec3 objectcolor = glm::vec3(1.0f, 1.0f, 1.0f));
    ~Firefly();

    DrawableObject* getVisual() { return visual; }
    Light* getLight() { return light; }

    void addToScene(Scene* scene) ;
};