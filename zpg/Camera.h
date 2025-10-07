#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class ShaderProgram;

class Camera {
private:
    glm::vec3 eye;
    glm::vec3 target;
    glm::vec3 up;
    float yaw;
    float pitch;

    std::vector<ShaderProgram*> observers; // Shadery, které sledují kameru

public:
    Camera(glm::vec3 position);

    void addObserver(ShaderProgram* shader);
    void notifyObservers();

    void processKeyboard(float deltaTime, bool forward, bool backward, bool left, bool right);
    void processMouse(float deltaX, float deltaY);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float aspectRatio) const;
};
