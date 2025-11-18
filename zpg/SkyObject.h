#pragma once
#include "ShaderProgram.h"
#include <glm/glm.hpp>

class SkyObject {
public:
    virtual ~SkyObject() = default;
    virtual void draw(const glm::mat4& view, const glm::mat4& projection) = 0;
    virtual ShaderProgram* getShader() const = 0;
};