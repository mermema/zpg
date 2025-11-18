#include "Flashlight.h"
Flashlight::Flashlight(Camera* cam)
    : ReflectorLight(glm::vec3(0.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(1.0f, 1.0f, 0.9f),
        2.0f,
        30.0f),
    camera(cam)
{
    camera->registerObserver(this);
}

void Flashlight::update(Observable* who) {
    setPosition(camera->getPosition());
    setDirection(glm::normalize(camera->getTarget()));
    cameraChangeFlag = true;
}

void Flashlight::setDirection(const glm::vec3& newDir) {
    direction = glm::normalize(newDir);
}

void Flashlight::setPosition(const glm::vec3& newPos) {
    delete transform;
    transform = new CompositeTransformation();
    transform->add(new Translation(newPos));
}

void Flashlight::notifyObservers()
{
    if (cameraChangeFlag)
    {
        Light::notifyObservers();
        cameraChangeFlag = false;
    }
}

void Flashlight::enable() {
    enabled = true;
    cameraChangeFlag = true;
    setIntensity(ONintensity);
    notifyObservers();
}

void Flashlight::setONIntesity(float value)
{
    ONintensity = value;
}

void Flashlight::disable() {
    enabled = false;
    cameraChangeFlag = true;
    setIntensity(0.0f);
    notifyObservers();
}

void Flashlight::toggle() {
    if (enabled) disable();
    else enable();
}

bool Flashlight::isEnabled() const {
    return enabled;
}
