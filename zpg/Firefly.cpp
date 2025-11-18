#include "Firefly.h"

Firefly::Firefly(Model* sharedModel, ShaderProgram* sharedShader,  Light* lightarg, CompositeTransformation* transformationarg, glm::vec3 objectcolor)
{
    transform = transformationarg;

    visual = new DrawableObject(sharedModel, sharedShader);
    visual->setTransformation(transform);
    visual->setObjectColor(objectcolor);

    light = lightarg;
    light->setTransform(transform);
}

Firefly::~Firefly() {
    delete visual;
    delete light;
    delete transform;
}

void Firefly::addToScene(Scene* scene) {
    scene->addObject(visual);
    scene->addLight(light);
}