#include "Light.h"
#include "ShaderProgram.h"
#include "Translation.h"

Light::Light( const glm::vec3& pos, const glm::vec3& col, float intens)
    : color(col), intensity(intens), index(-1)
{
    //cration of basic transformation with translation to target position
    transform = new CompositeTransformation();
    transform->add(new Translation(pos));
}

Light::~Light() {
    delete transform;
}

void Light::setTransform(CompositeTransformation* t) {
    delete transform;
    transform = t;
    hasDynamicTransformation = t->hasDynamicTransformations();
    notifyObservers();
}

void Light::setPosition(const glm::vec3& newPos) {
    delete transform;
    transform = new CompositeTransformation();
    transform->add(new Translation(newPos));
    notifyObservers();
}

glm::vec3 Light::getPosition() const {
    if (!transform) return glm::vec3(0.0f);

    glm::mat4 matrix = transform->getMatrix();
    //translations are in third row
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

void Light::applyToShader(ShaderProgram* shader) const {

    if (index == -1) {
        cout << "Light has not set index!!!" << endl;
    }
    glm::mat4 lightMatrix = transform->getMatrix();

    shader->use();
    shader->setLightUniforms(index, type, color, lightMatrix, intensity, constant, linear, quadratic);
    shader->unset();

    
    }


void Light::notifyObservers() {
    //std::cout << "Light::notifyObservers() - Notifying " << observers.size() << " observers" << std::endl;

    for (auto& observer : observers) {
        observer->update(this);
    }
}

void Light::setAttenuation(float constant, float linear, float quadratic)
{
    this->linear = linear;
    this->constant = constant;
    this->quadratic = quadratic;
}
void Light::setIndex(int idx)
{
    this->index = idx;
}