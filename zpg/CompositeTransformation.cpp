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

void CompositeTransformation::update(float time) {
    for (auto* t : transformations) {
        if (auto* dynobj = dynamic_cast<DynamicTransformation*>(t)) {
            dynobj->update(time); 
        }
    }
}



glm::mat4 CompositeTransformation::getMatrix() const {
    glm::mat4 result(1.0f);
    for (auto t : transformations)
        result = result * t->getMatrix();
    return result;
}
