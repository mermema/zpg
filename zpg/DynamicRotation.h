#pragma once
#include "Rotation.h"
#include <GLFW/glfw3.h>
#include "DynamicTransformation.h"
class DynamicRotation : public Rotation, public DynamicTransformation {
public:
    DynamicRotation(const glm::vec3& axis, float timefactor = 1) : Rotation(0.0f, axis) { this->timefactor = timefactor; }



    glm::mat4 getMatrix() const override {
        return  glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * this->timefactor, axis);
    }
    
private:
    float timefactor;

};
