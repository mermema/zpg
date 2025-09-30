#pragma once
#include <vector>
#include <glm/vec3.hpp>

using namespace std;

class ShapeBuilder {
public:
    static vector<glm::vec3> makeTriangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
    static vector<glm::vec3> makeSquare(const glm::vec3& bottomLeft, const glm::vec3& topRight);
    static vector<glm::vec3> makePolygon(const vector<glm::vec3>& points);
    static vector<glm::vec3> makeCube(const glm::vec3& minCorner, const glm::vec3& maxCorner);

private:
    ShapeBuilder() = delete;

};
