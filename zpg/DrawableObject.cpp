#include "DrawableObject.h"
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

DrawableObject::DrawableObject(Model* model, ShaderProgram* shader)
    : model(model), shader(shader), transform(nullptr) {
}

void DrawableObject::setTransformation(CompositeTransformation* t) {
    transform = t;
}

CompositeTransformation* DrawableObject::getTransformation() const {
    return transform;
}

void DrawableObject::update(float time) {
    if (transform)
        transform->update(time);
}

void DrawableObject::draw() const {
    shader->use();

    if (transform) {
        glm::mat4 m = transform->getMatrix();
        shader->set("model", m);
    }
    else {
        shader->set("model", glm::mat4(1.0f));
    }

    model->draw();
}