#pragma once
#include "DynamicBezierCurve.h"
#include <vector>


class DynamicBezierSpline :
    public DynamicBezierCurve
{
public:

    DynamicBezierSpline(std::vector<glm::vec3> pts, float timefactor = 1.0f)
        : DynamicBezierCurve(glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec3(0), timefactor)
    {
        this->allPoints = pts;
        this->startTime = glfwGetTime();

    }


    glm::mat4 getMatrix() const override {

        int segmentCount = ((int)allPoints.size() - 1) / 3;
        if (segmentCount <= 0) return glm::mat4(1.0f);

        float t = (glfwGetTime()-startTime) * this->timefactor;

        bool reverse = false;
        float u = fmod(t, 2.0f);
        if (u > 1.0f) 
        { 
            u = 2 - u; 
            reverse = true; 
        }

        float s = u * segmentCount;
        int segIndex = floor(s);
        if (segIndex >= segmentCount)
        {
            segIndex = segmentCount - 1;
        }

        float localT = s - segIndex;

        int i = segIndex * 3;
        if (reverse) {
            points[0] = allPoints[i + 3];
            points[1] = allPoints[i + 2];
            points[2] = allPoints[i + 1];
            points[3] = allPoints[i + 0];
            localT = 1.0f - localT;
        }
        else {
            points[0] = allPoints[i + 0];
            points[1] = allPoints[i + 1];
            points[2] = allPoints[i + 2];
            points[3] = allPoints[i + 3];
        }

        glm::vec3 position = calculateBezierPoint(localT);
        glm::vec3 tangent = calculateBezierTangent(localT);

        tangent = glm::normalize(tangent);

        float angle = atan2(tangent.x, tangent.z);

        glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0));

        float angleVer = atan2(tangent.y, sqrt(tangent.z * tangent.z + tangent.x * tangent.x));
        glm::mat4 rotationVer = glm::rotate(glm::mat4(1.0f), -angleVer, glm::vec3(1.0f, 0.0f, 0.0f));

        return translation * rotation * rotationVer;
    }



private:

    std::vector<glm::vec3> allPoints;
    float startTime;
};

