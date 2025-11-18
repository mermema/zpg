#include "ReflectorLight.h"
#include "ShaderProgram.h"

void ReflectorLight::applyToShader(ShaderProgram* shader) const {
    Light::applyToShader(shader);
    shader->use();
    shader->setLightDirection(getIndex(), direction);
    shader->setLightAngle(getIndex(), angle);
    shader->unset();

}