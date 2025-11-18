#pragma once
#include "ReflectorLight.h"
#include "Translation.h"
#include "Camera.h"
#include <iostream>

class Flashlight : public ReflectorLight, public Observer {
private:
    Camera* camera;
    bool enabled = true;
    float ONintensity = 3.0f;
    bool cameraChangeFlag = true;

public:
    Flashlight(Camera* cam);

    void update(Observable* who) override;
    void setDirection(const glm::vec3& newDir) override;
    void setPosition(const glm::vec3& newPos);

    void notifyObservers() override;

    void enable();
    void setONIntesity(float value);
    void disable();
    void toggle();

    bool isEnabled() const;
};
