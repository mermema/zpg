#pragma once
#include "Light.h"

class PointLight : public Light {
public:
    PointLight(const glm::vec3& pos = glm::vec3(0.0f),
        const glm::vec3& col = glm::vec3(1.0f),
        float intens = 1.0f)
        : Light(pos, col, intens) {
        setType(1);
        setAttenuation(1.0f, 0.1f, 0.3f);
    }
};