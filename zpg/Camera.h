#pragma once
#include "Observable.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderProgram.h"

class ShaderProgram;

class Camera : public Observable {
private:
    glm::vec3 eye;       //pozice kamery
    glm::vec3 target;    //pozice toho na co se vivame
    glm::vec3 up;        //smer

         //vertikalnirotace
    float fov;           //zorný úhel
    float zNear, zFar;   

    bool enabled = true;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    float aspectRatio = 4.0f / 3.0f;

public:
    float yaw;           //horizintalnirotace
    float pitch;
    Camera(float fov = 45.0f, float zNear = 0.1f, float zFar = 100.0f);

    //Pohyb
    void forward(float rate);
    void backward(float rate);
    void toLeft(float rate);
    void toRight(float rate);

    //Otoèení
    void changeYaw(float deg);
    void changePitch(float deg);

    // Aktualizace smìru a pohledu
    void recalculateTarget();
    void calculateViewMatrix();
    void setAspectRatio(float newAspect);

    glm::vec3 getTarget() const;
    bool getStatus() const;

    // Aktivace/deaktivace
    void enable();
    void disable();

    // Pøi notifikaci nejdøív pøepoèítá viewMatrix
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
