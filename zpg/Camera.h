#pragma once
#include "Observable.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderProgram.h"

class ShaderProgram;

class Camera : public Observable {
private:
    glm::vec3 eye;       //camera pos
    glm::vec3 target;    //target vector
    glm::vec3 up;        //smer

    float fov;           //field of wiew
    float zNear, zFar;   //near and far target

    bool enabled = true;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    float aspectRatio = 4.0f / 3.0f;

public:
    float yaw;           //horizontalization
    float pitch;
    Camera(float fov = 45.0f, float zNear = 0.1f, float zFar = 100.0f);

    //moving
    void forward(float rate);
    void backward(float rate);
    void toLeft(float rate);
    void toRight(float rate);

    void changeYaw(float deg);
    void changePitch(float deg);

    //recalculation
    void recalculateTarget();
    void calculateViewMatrix();
    void setAspectRatio(float newAspect);

    glm::vec3 getTarget() const;
    bool getStatus() const;

    void enable();
    void disable();

    void notifyObservers() override;

    void setPosition(const glm::vec3& newPosition) {
        eye = newPosition;
        notifyObservers();
    }

    glm::vec3 getPosition() {
        return eye; 
    }
    void applyToShader(ShaderProgram* shader);

    void setTarget(const glm::vec3& newTarget);
    
    glm::mat4 getViewMatrix() const {
        return viewMatrix;
    }

    glm::mat4 getProjectionMatrix() const {
        return projectionMatrix;
    }

};
