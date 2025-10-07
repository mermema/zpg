#include "DrawableObject.h"
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

DrawableObject::DrawableObject(Model* model, ShaderProgram* shader)
    : model(model), shader(shader), transform(nullptr) {
}

void DrawableObject::setTransformation(Transformation* t) {
    transform = t;
}

Transformation* DrawableObject::getTransformation() const {
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
    model->draw();
}

void DrawableObject::draw(const glm::mat4& view, const glm::mat4& projection) const {
    shader->use();

    if (transform) {
        glm::mat4 modelMatrix = transform->getMatrix();
        shader->set("modelMatrix", modelMatrix);
    }

    // Posíláme kameru
    shader->set("viewMatrix", view);
    shader->set("projectionMatrix", projection);

    model->draw();
}

// varianta s kamerou – používaná ve Scene::draw(Camera*)
void DrawableObject::draw(Camera* camera) const {
    shader->use();

    // Model matrix
    if (transform) {
        glm::mat4 modelMatrix = transform->getMatrix();
        shader->set("modelMatrix", modelMatrix);
    }

    // View a Projection z kamery
    if (camera) {
        shader->set("viewMatrix", camera->getViewMatrix());
        shader->set("projectionMatrix", camera->getProjectionMatrix(16.0f / 9.0f));
    }

    model->draw();
}
