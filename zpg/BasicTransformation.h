#pragma once
#include <glm/mat4x4.hpp>

class BasicTransformation {
public:
    virtual ~BasicTransformation() = default;
    virtual glm::mat4 getMatrix() const = 0;

};
