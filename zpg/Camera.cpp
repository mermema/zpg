#include "Camera.h"
#include "ShaderProgram.h"

Camera::Camera(glm::vec3 position)
    : eye(position), up(0, 1, 0), yaw(-90.0f), pitch(0.0f) {
}

void Camera::addObserver(ShaderProgram* shader) {
    observers.push_back(shader);
}

void Camera::notifyObservers() {
    for (auto* shader : observers) {
        shader->use();
        shader->set("viewMatrix", getViewMatrix());
        shader->set("projectionMatrix", getProjectionMatrix(800.0f / 600.0f)); // nebo pøedat aspect ratio dynamicky
    }
}

glm::mat4 Camera::getViewMatrix() const {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    return glm::lookAt(eye, eye + glm::normalize(front), up);
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio) const {
    return glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
}

void Camera::processKeyboard(float deltaTime, bool forward, bool backward, bool left, bool right) {
    float speed = 2.5f * deltaTime;
    glm::vec3 front = glm::normalize(glm::vec3(
        cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
        sin(glm::radians(pitch)),
        sin(glm::radians(yaw)) * cos(glm::radians(pitch))
    ));
    glm::vec3 rightVec = glm::normalize(glm::cross(front, up));

    if (forward) eye += speed * front;
    if (backward) eye -= speed * front;
    if (left) eye -= speed * rightVec;
    if (right) eye += speed * rightVec;

    notifyObservers(); // po pohybu hned aktualizujeme shadery
}

void Camera::processMouse(float dx, float dy) {
    float sensitivity = 0.1f;
    yaw += dx * sensitivity;
    pitch += dy * sensitivity;
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    notifyObservers();
}
