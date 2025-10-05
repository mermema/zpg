#pragma once
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <vector>
using namespace std;

class Model {
    GLuint VAO, VBO;
    int vertexCount;
public:
    Model();
    ~Model();


    void upload(const vector<glm::vec3>& vertices);
    void upload(const float* data, size_t vertexCount_);
    void draw();
};
