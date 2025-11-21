#pragma once
#include <glm/glm.hpp>
#include "ShaderProgram.h"

class Material {
private:
    glm::vec3 ambient;      
    glm::vec3 diffuse;       
    glm::vec3 specular;     
    float shininess;        

public:
    Material(glm::vec3 amb = glm::vec3(0.05f),
        glm::vec3 diff = glm::vec3(1.0f),
        glm::vec3 spec = glm::vec3(0.8f),
        float shine = 32.0f)
        : ambient(amb), diffuse(diff), specular(spec), shininess(shine) {
    }

    void applyToShader(ShaderProgram* shader) {
        
        shader->setMaterial(ambient, diffuse, specular, shininess);
    }

    glm::vec3 getAmbient() const { return ambient; }
    glm::vec3 getDiffuse() const { return diffuse; }
    glm::vec3 getSpecular() const { return specular; }
    float getShininess() const { return shininess; }
    bool loadFromMTL(const std::string& filepath);

};