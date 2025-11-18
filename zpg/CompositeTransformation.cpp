#include "CompositeTransformation.h"
#include "DynamicTransformation.h"

CompositeTransformation::~CompositeTransformation() {
    for (auto t : transformations)
        delete t;
    transformations.clear();
}

void CompositeTransformation::add(BasicTransformation* t) {
    transformations.push_back(t);
}

void CompositeTransformation::insert(BasicTransformation* t) {
    transformations.insert(transformations.begin(), t); 
}

glm::mat4 CompositeTransformation::getMatrix() const {
    glm::mat4 result(1.0f);
    for (auto t : transformations)
        result = t->getMatrix() * result;
    return result;
}

bool CompositeTransformation::hasDynamicTransformations() {
    for (auto* trans : transformations) {
        if (dynamic_cast<DynamicTransformation*>(trans)) {
            return true;
        }
    }
    return false;
}