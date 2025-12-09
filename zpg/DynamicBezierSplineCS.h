#pragma once
#include "DynamicBezierCurve.h"
#include <vector>

class DynamicBezierSplineCS : public DynamicBezierCurve
{
public:

    DynamicBezierSplineCS(std::vector<glm::vec3> pts, float timefactor = 1.0f)
        : DynamicBezierCurve(glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec3(0), timefactor)
    {
        this->startTime = glfwGetTime();
        this->allPoints = pts;
        computeSegmentLengths();
    }


    glm::mat4 getMatrix() const override
    {
        int segmentCount = (allPoints.size() - 1) / 3;
        if (segmentCount <= 0)
            return glm::mat4(1.0f);

        float t = (glfwGetTime()-startTime) * this->timefactor;

        bool reverse = false;
        float u = fmodf(t, 2.0f);
        if (u > 1.0f) {
            u = 2.0f - u;
            reverse = true;
        }


        int segIndex = 0;
        for (int s = 0; s < segmentCount; s++)
        {
            if (u >= cumulative[s] && u <= cumulative[s + 1]) {
                segIndex = s;
                break;
            }
        }

        float segStart = cumulative[segIndex];
        float segEnd = cumulative[segIndex + 1];
        float localT = (u - segStart) / (segEnd - segStart);


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
    float startTime;
    std::vector<glm::vec3> allPoints;
    std::vector<float> segmentLengths;
    std::vector<float> cumulative;
    float totalLength = 0.0f;



    void computeSegmentLengths()
    {
        int segmentCount = (allPoints.size() - 1) / 3;

        segmentLengths.resize(segmentCount);
        totalLength = 0.0f;

        for (int s = 0; s < segmentCount; s++)
        {
            int i = s * 3;
            float L = computeSegmentLength(
                allPoints[i + 0],
                allPoints[i + 1],
                allPoints[i + 2],
                allPoints[i + 3]
            );
            segmentLengths[s] = L;
            totalLength += L;
        }

        cumulative.resize(segmentCount + 1);
        cumulative[0] = 0.0f;

        float acc = 0.0f;
        for (int i = 0; i < segmentCount; i++)
        {
            acc += segmentLengths[i] / totalLength;
            cumulative[i + 1] = acc;
        }

        cumulative[segmentCount] = 1.0f;
    }


    float computeSegmentLength(glm::vec3 P0, glm::vec3 P1,
        glm::vec3 P2, glm::vec3 P3)
    {
        const int N = 40;
        float length = 0.0f;

        glm::vec3 prev = P0;

        for (int i = 1; i <= N; i++)
        {
            float t = (float)i / (float)N;
            glm::vec3 p = bezierPointRaw(P0, P1, P2, P3, t);
            length += glm::length(p - prev);
            prev = p;
        }

        return length;
    }


    glm::vec3 bezierPointRaw(glm::vec3 a, glm::vec3 b,
        glm::vec3 c, glm::vec3 d, float t)
    {
        float u = 1.0f - t;
        float u2 = u * u;
        float u3 = u2 * u;
        float t2 = t * t;
        float t3 = t2 * t;

        return 
            a * u3 +
            b * (3.0f * u2 * t) +
            c * (3.0f * u * t2) +
            d * t3;
    }
};
