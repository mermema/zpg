#pragma once
#include "Translation.h"
#include "DynamicTransformation.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

class DynamicTranslation : public Translation, public DynamicTransformation {
public:
    DynamicTranslation(const glm::vec3& P0, const glm::vec3& direction, float timefactor = 1.0f)
        : Translation(glm::vec3(0.0f)), P0(P0), dir(direction), timefactor(timefactor) {
    }

    glm::mat4 getMatrix() const override {
        float t = glfwGetTime() * timefactor;

        //make u in range 0->1->0....
        float u = fmod(t, 2.0f);
        if (u > 1.0f) { u = 2.0f - u;}

        glm::vec3 pos = P0 + (dir * u);

        return glm::translate(glm::mat4(1.0f), pos);
    }

private:
    glm::vec3 P0;       //startpoint
    glm::vec3 dir;      //direction vector
    float timefactor;   //speed devidor
};
