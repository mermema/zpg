#pragma once
#include "Light.h"

class AmbientLight : public Light {
public:
    AmbientLight(const glm::vec3& col = glm::vec3(1.0f), float intens = 0.2f)
        : Light(glm::vec3(0.0f), col, intens) {
        setType(0);
        setAttenuation(1.0f, 0.0f, 0.0f);
    }
};