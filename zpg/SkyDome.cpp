#include "SkyDome.h"
#include <iostream>

SkyDome::SkyDome(ShaderProgram* shaderProgram, const std::string& objPath, const std::string& texturePath)
    : Texture(Material(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 1.0f)),
    shader(shaderProgram) {

    // Naètení OBJ modelu
    domeModel = new ObjModel(objPath.c_str());

    // Naètení 2D textury (na rozdíl od cubemapu u SkyBoxu)
    loadTexture(texturePath);

    std::cout << "SkyDome created from OBJ: " << objPath << std::endl;
}

SkyDome::~SkyDome() {
    delete domeModel;
}

void SkyDome::applyToShader(ShaderProgram* shader) {
    shader->use();

   
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        shader->setSkyDomeTexture(0);  
        shader->unset();

    
}

void SkyDome::draw(const glm::mat4& view, const glm::mat4& projection) {
    if (!shader || !domeModel) return;

    shader->use();

    glm::mat4 viewWithoutTranslation = glm::mat4(glm::mat3(view)); //ignore translations

    shader->setViewMatrix(viewWithoutTranslation);
    shader->setProjectionMatrix(projection);

    applyToShader(shader);

    //render skydome model
    domeModel->draw();
    shader->unset();
}