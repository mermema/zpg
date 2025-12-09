#pragma once
#include "Translation.h"
#include "DynamicTransformation.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

class DynamicTranslationTwoSegment : public Translation, public DynamicTransformation {
public:
    DynamicTranslationTwoSegment(glm::vec3 P0,
        glm::vec3 v1,
        glm::vec3 v2,
        float timefactor = 1.0f)
        : Translation(glm::vec3(0.0f)), P0(P0), v1(v1), v2(v2), timefactor(timefactor)
    {
        len1 = glm::length(v1);
        len2 = glm::length(v2);
        totalLen = len1 + len2;
    }

    glm::mat4 getMatrix() const override {
        float t = glfwGetTime() * timefactor;

        float u = fmod(t, 2.0f);
        if (u > 1.0f) { u = 2.0f - u; }

        float d = u * totalLen;

        glm::vec3 pos;

        if (d <= len1) {
            float localT = d / len1;
            pos = P0 + v1 * localT;
        }
        else {
            float d2 = d - len1;
            float localT = d2 / len2;
            pos = P0 + v1 + v2 * localT;
        }

        return glm::translate(glm::mat4(1.0f), pos);
    }

private:
    glm::vec3 P0;
    glm::vec3 v1;
    glm::vec3 v2;

    float len1;
    float len2;
    float totalLen;
    float timefactor;
};
