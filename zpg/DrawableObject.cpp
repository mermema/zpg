#include "DrawableObject.h"
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

int DrawableObject::nextID = 1;



DrawableObject::DrawableObject(Model* model, ShaderProgram* shader, Texture* texturearg)
    : model(model), shader(shader), transform(nullptr), texture(texturearg), objectID(nextID++) {
}

void DrawableObject::setTransformation(CompositeTransformation* t) {
    transform = t;
}
void DrawableObject::setTexture(Texture* tex) { texture = tex; }
Texture* DrawableObject::getTexture() const { return texture; }

CompositeTransformation* DrawableObject::getTransformation() const {
    return transform;
}

void DrawableObject::draw() const {
    shader->use();

    if (transform) {
        glm::mat4 m = transform->getMatrix();
        shader->setModelMatrix(m);
    }
    else {
        shader->setModelMatrix(glm::mat4(1.0f));
    }

    if (texture) {
        texture->applyToShader(shader);
    }
    else {
        shader->setHasTexture(0);
        shader->setObjectColor(objectColor);
    }

    model->draw();
    shader->unset();
}
void DrawableObject::setObjectColor(glm::vec3 color)
{
    this->objectColor = color;
}