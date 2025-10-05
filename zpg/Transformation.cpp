#include "Transformation.h"

Transformation::~Transformation() {
    for (auto t : transformations)
        delete t;
    transformations.clear();
}

void Transformation::add(BasicTransformation* t) {
    transformations.push_back(t);
}

void Transformation::update(float time) {
    for (auto t : transformations)
        t->update(time);
}

glm::mat4 Transformation::getMatrix() const {
    glm::mat4 result(1.0f);
    for (auto t : transformations)
        result = result * t->getMatrix();
    return result;
}
