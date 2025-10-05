#include "ShapeBuilder.h"

vector<glm::vec3> ShapeBuilder::makeTriangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) {
    return { a, b, c };
}

vector<glm::vec3> ShapeBuilder::makeSquare(const glm::vec3& bottomLeft, const glm::vec3& topRight) {
    glm::vec3 bottomRight(topRight.x, bottomLeft.y, bottomLeft.z);
    glm::vec3 topLeft(bottomLeft.x, topRight.y, topRight.z);

    return { bottomLeft, bottomRight, topLeft, bottomRight, topRight, topLeft };
}

vector<glm::vec3> ShapeBuilder::makePolygon(const vector<glm::vec3>& points) {
    vector<glm::vec3> triangles;
    if (points.size() < 3) return triangles;

    for (size_t i = 1; i + 1 < points.size(); i++) {
        triangles.push_back(points[0]);
        triangles.push_back(points[i]);
        triangles.push_back(points[i + 1]);
    }
    return triangles;
}

vector<glm::vec3> ShapeBuilder::makeCube(const glm::vec3& minCorner, const glm::vec3& maxCorner) {
    vector<glm::vec3> verts;

    glm::vec3 blb(minCorner.x, minCorner.y, minCorner.z);
    glm::vec3 brb(maxCorner.x, minCorner.y, minCorner.z);
    glm::vec3 tlb(minCorner.x, maxCorner.y, minCorner.z);
    glm::vec3 trb(maxCorner.x, maxCorner.y, minCorner.z);

    glm::vec3 blf(minCorner.x, minCorner.y, maxCorner.z);
    glm::vec3 brf(maxCorner.x, minCorner.y, maxCorner.z);
    glm::vec3 tlf(minCorner.x, maxCorner.y, maxCorner.z);
    glm::vec3 trf(maxCorner.x, maxCorner.y, maxCorner.z);

    // Spodní stìna
    verts.insert(verts.end(), { blb, brb, tlb, brb, trb, tlb });
    // Horní stìna
    verts.insert(verts.end(), { blf, tlf, brf, brf, tlf, trf });
    // Pøední stìna
    verts.insert(verts.end(), { blf, brf, blb, brf, brb, blb });
    // Zadní stìna
    verts.insert(verts.end(), { tlf, trf, tlb, trf, trb, tlb });
    // Levá stìna
    verts.insert(verts.end(), { blf, blb, tlf, blb, tlb, tlf });
    // Pravá stìna
    verts.insert(verts.end(), { brf, trf, brb, trf, trb, brb });

    return verts;
}
