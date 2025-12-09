#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "BasicTransformation.h"
#include "DynamicTransformation.h"

class CompositeTransformation :public BasicTransformation {
public:
    CompositeTransformation() = default;
    ~CompositeTransformation() override;

    void add(BasicTransformation* t);
    void insert(BasicTransformation* t);
    void remove(BasicTransformation* t);
    glm::mat4 getMatrix() const;


    bool hasDynamicTransformations();

private:
    std::vector<BasicTransformation*> transformations;
};
