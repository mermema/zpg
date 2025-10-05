#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "BasicTransformation.h"

class Transformation {
public:
    Transformation() = default;
    ~Transformation();

    void add(BasicTransformation* t);
    void update(float time);
    glm::mat4 getMatrix() const;

private:
    std::vector<BasicTransformation*> transformations;
};
