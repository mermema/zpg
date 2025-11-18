#pragma once
#include "Texture.h"
#include "ShaderProgram.h"
#include "SkyObject.h"
#include "ObjModel.h"
#include <string>

class SkyDome : public Texture, public SkyObject {
private:
    ObjModel* domeModel;
    ShaderProgram* shader;

public:
    SkyDome(ShaderProgram* shaderProgram, const std::string& objPath, const std::string& texturePath);
    ~SkyDome();

    void applyToShader(ShaderProgram* shader);
    //void loadTexture(const std::string& texturePath);
    void draw(const glm::mat4& view, const glm::mat4& projection) override;
    ShaderProgram* getShader() const override { return shader; }
};