#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "BasicTransformation.h"

class CompositeTransformation :public BasicTransformation {
public:
    CompositeTransformation() = default;
    ~CompositeTransformation() override;

    void add(BasicTransformation* t);
    void update(float time);
    glm::mat4 getMatrix() const;

private:
    std::vector<BasicTransformation*> transformations;
};
