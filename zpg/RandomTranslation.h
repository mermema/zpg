#pragma once
#include "DynamicTransformation.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <random>
#include "BasicTransformation.h"

class RandomTranslation : public DynamicTransformation, public BasicTransformation {
public:
    RandomTranslation(float radius = 0.1f, float changeInterval = 6.0f, float movementSpeed = 2.0f)
        : radius(radius), changeInterval(changeInterval), movementSpeed(movementSpeed) {
        std::random_device rd;
        gen = std::mt19937(rd());
        dist = std::uniform_real_distribution<float>(-1.0f, 1.0f);

        currentPosition = glm::vec3(0.0f);
        targetDirection = generateRandomDirection();
        lastChangeTime = glfwGetTime();
    }

    glm::mat4 getMatrix() const override {
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastUpdateTime;
        lastUpdateTime = currentTime;

        if (currentTime - lastChangeTime >= changeInterval) {
            targetDirection = generateRandomDirection();
            lastChangeTime = currentTime;
        }

        currentPosition += targetDirection * movementSpeed * (float)deltaTime;

        if (glm::length(currentPosition) > radius) {
            currentPosition = glm::normalize(currentPosition) * radius;

            targetDirection = generateRandomDirection();
        }

        return glm::translate(glm::mat4(1.0f), currentPosition);
    }

private:
    glm::vec3 generateRandomDirection() const {
        glm::vec3 direction;
        do {
            direction = glm::vec3(dist(gen), dist(gen), dist(gen));
        } while (glm::length(direction) < 0.1f); 

        return glm::normalize(direction);
    }

    float radius;
    float changeInterval;
    float movementSpeed;
    mutable glm::vec3 currentPosition;
    mutable glm::vec3 targetDirection;
    mutable double lastChangeTime;
    mutable double lastUpdateTime = glfwGetTime();
    mutable std::mt19937 gen;
    mutable std::uniform_real_distribution<float> dist;
};