#pragma once
#include "Translation.h"
#include "DynamicTransformation.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

class DynamicBezierCurve : public Translation, public DynamicTransformation {
public:
    DynamicBezierCurve( glm::vec3 P0,  glm::vec3 P1,
         glm::vec3 P2,  glm::vec3 P3,
        float timefactor = 1.0f)
        : Translation(glm::vec3(0.0f)), points{ P0, P1, P2, P3 }, timefactor(timefactor) {
    }

    glm::mat4 getMatrix() const override {
        float t = glfwGetTime() * timefactor;


        bool reverse = false;

        float u = fmod(t, 2.0f);
        if (u > 1.0f) {
            u = 2.0f - u;
            reverse = true;
        }


        glm::vec3 position = calculateBezierPoint(u);

        glm::vec3 tangent = calculateBezierTangent(u);


        if (reverse)
        {
            tangent = -tangent; //or angle += glm::radians(180.0); after its definition
        }

        tangent = glm::normalize(tangent);//not nessesary

        //arctan of x z to get angle
        float angle = atan2(tangent.x, tangent.z);


        glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));

        float angleVer = atan2(tangent.y, sqrt(tangent.z * tangent.z + tangent.x * tangent.x));
        glm::mat4 rotationVer = glm::rotate(glm::mat4(1.0f), -angleVer, glm::vec3(1.0f, 0.0f, 0.0f));

        return translation * rotation * rotationVer;
    }

private:

protected:
    float timefactor;
    mutable glm::vec3 points[4];

    //exact point on bazier curve
    glm::vec3 calculateBezierPoint(float t) const {
        float u = 1.0f - t;
        float u2 = u * u;
        float u3 = u2 * u;
        float t2 = t * t;
        float t3 = t2 * t;

        glm::vec3 bp = 
            u3 * points[0] +
            3.0f * u2 * t * points[1] +
            3.0f * u * t2 * points[2] +
            t3 * points[3];
        return bp;
    }

    //tangent angle
    glm::vec3 calculateBezierTangent(float t) const {
        float u = 1.0f - t;
        float u2 = u * u;
        float t2 = t * t;

        glm::vec3 tangent = //1 derivation of bazier curve
            3.0f * u2 * (points[1] - points[0]) +
            6.0f * u * t * (points[2] - points[1]) +
            3.0f * t2 * (points[3] - points[2]);
        return tangent;
    }
};