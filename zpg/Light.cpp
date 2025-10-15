#include "Light.h"
#include "ShaderProgram.h"
#include "Translation.h"

Light::Light(const glm::vec3& pos, const glm::vec3& col, float intens)
    : color(col), intensity(intens)
{
    // Vytvoøíme základní transformaci s pozicí
    transform = new CompositeTransformation();
    transform->add(new Translation(pos));
}

Light::~Light() {
    delete transform;
}

void Light::setTransform(CompositeTransformation* t) {
    delete transform;
    transform = t;
    notifyObservers();
}

void Light::setPosition(const glm::vec3& newPos) {
    // Jednodušší zpùsob - vytvoøíme novou transformaci
    delete transform;
    transform = new CompositeTransformation();
    transform->add(new Translation(newPos));
    notifyObservers();
}

glm::vec3 Light::getPosition() const {
    if (!transform) return glm::vec3(0.0f);

    glm::mat4 matrix = transform->getMatrix();
    // Pozice je v posledním sloupci matice
    return glm::vec3(matrix[3]);
}

void Light::setColor(const glm::vec3& newColor) {
    color = newColor;
    notifyObservers();
}

void Light::setIntensity(float newIntensity) {
    intensity = newIntensity;
    notifyObservers();
}

void Light::applyToShader(ShaderProgram* shader, const std::string& uniformName) const {
    auto position = getPosition();
    shader->set(uniformName + ".position", position);
    shader->set(uniformName + ".color", color);
    shader->set(uniformName + ".intensity", intensity);
}

void Light::update(float deltaTime) {
    if (transform) {
        transform->update(deltaTime);
    }
}

void Light::notifyObservers() {
    for (auto& observer : observers) {
        observer->update(this);
    }
}