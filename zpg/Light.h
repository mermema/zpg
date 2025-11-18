#pragma once
#include <glm/glm.hpp>
#include "Observable.h"
#include "CompositeTransformation.h"
#include <string>
class ShaderProgram;

class Light : public Observable {
private:
    int index;
    int type = 1;           
    glm::vec3 color;
    float intensity;
    float constant = 1;
    float linear = 0;
    float quadratic = 0;
    bool hasDynamicTransformation;
protected:
    CompositeTransformation* transform;


public:
    Light( const glm::vec3& pos = glm::vec3(0.0f),
        const glm::vec3& col = glm::vec3(1.0f),
        float intens = 1.0f);

    ~Light();

    void setTransform(CompositeTransformation* t);
    CompositeTransformation* getTransform() const { return transform; }

    virtual void setPosition(const glm::vec3& newPos);
    glm::vec3 getPosition() const;

    void setColor(const glm::vec3& newColor);
    void setIntensity(float newIntensity);

    glm::vec3 getColor() const { return color; }
    float getIntensity() const { return intensity; }

    virtual void applyToShader(ShaderProgram* shader) const;
    void notifyObservers() override;
    void setType(int type) { this->type = type; notifyObservers(); }
    int getType() const { return type; }
    void setAttenuation(float constant, float linear, float quadratic);

    int getIndex() const { return index; }
    void setIndex(int idx);
    bool hasDynamic() { return hasDynamicTransformation; }
};