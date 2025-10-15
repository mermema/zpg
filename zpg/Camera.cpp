#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>
#include <iostream>

Camera::Camera(float fov, float zNear, float zFar)
    : fov(fov), zNear(zNear), zFar(zFar),
    eye(0.0f, 0.0f, 5.0f),
    up(0.0f, 1.0f, 0.0f),
    yaw(-90.0f), pitch(0.0f)
{
    recalculateTarget();
    calculateViewMatrix();
    projectionMatrix = glm::perspective(glm::radians(fov), 800.0f / 600.0f, zNear, zFar);

}

void Camera::recalculateTarget() {
    double radPitch = glm::radians(this->pitch);
    double radYaw = glm::radians(this->yaw);

    target.x = std::cos(radPitch) * std::cos(radYaw);
    target.z = std::cos(radPitch) * std::sin(radYaw);
    target.y = std::sin(radPitch);

    notifyObservers(); // Informuj pozorovatele
}

void Camera::calculateViewMatrix() {
    viewMatrix = glm::lookAt(eye, eye + target, up);
}

void Camera::forward(float rate) {
    eye += glm::normalize(target) * rate;
    notifyObservers();
}

void Camera::backward(float rate) {
    eye -= glm::normalize(target) * rate;
    notifyObservers();
}

void Camera::toLeft(float rate) {
    eye -= glm::normalize(glm::cross(target, up)) * rate;
    notifyObservers();
}

void Camera::toRight(float rate) {
    eye += glm::normalize(glm::cross(target, up)) * rate;
    notifyObservers();
}

void Camera::changeYaw(float deg) {
    yaw += deg;
    if (yaw >= 360.f) yaw -= 360.f;
    if (yaw <= -360.f) yaw += 360.f;

    recalculateTarget();
}

void Camera::changePitch(float deg) {
    pitch += deg;
    if (pitch > 89.f) pitch = 89.f;
    if (pitch < -89.f) pitch = -89.f;

    recalculateTarget();
}
/*

glm::mat4 Camera::getViewMatrix() const {
    return viewMatrix;
}

glm::vec3 Camera::getPosition() const {
    return eye;
}
*/


glm::vec3 Camera::getTarget() const {
    return target;
}
void Camera::enable() {
    enabled = true;
}

void Camera::disable() {
    enabled = false;
}

bool Camera::getStatus() const {
    return enabled;
}

void Camera::applyToShader(ShaderProgram* shader) {
    shader->use();
    shader->set("view", viewMatrix);
    shader->set("projection", projectionMatrix);
    shader->set("viewPos", eye);
}

void Camera::notifyObservers() {
    calculateViewMatrix();
    std::cout << "Camera notifying " << observers.size() << " observers" << std::endl;
    for (Observer* observer : observers) {
        std::cout << "Notifying observer..." << std::endl;
        observer->update(this);
    }
}