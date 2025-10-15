#pragma once
#include "Rotation.h"
#include "DynamicTransformation.h"
#include <GLFW/glfw3.h>

class DynamicRotation : public Rotation, public DynamicTransformation {
public:
    DynamicRotation(const glm::vec3& axis, float timefactor = 1) : Rotation(0.0f, axis) { this->timefactor = timefactor; }

    void update(float time) override {
        modelMatrix = glm::rotate(glm::mat4(1.0f), time * this->timefactor, axis);
    }

    void update() {
        modelMatrix = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * this->timefactor, axis);
    }

private:
    float timefactor;

};
