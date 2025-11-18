#pragma once
#include "Material.h"
#include "ShaderProgram.h"
#include <string>
#include <iostream>
#include "stb_image.h" 

class Texture {
private:
    Material material;

protected: 
    GLuint textureID;
    bool hasTexture;


public:
    Texture(const Material& mat)
        : material(mat), textureID(0), hasTexture(false) {
    }

    Texture(const Material& mat, const std::string& texturePath)
        : material(mat), hasTexture(true) {
        loadTexture(texturePath);
    }

    void loadTexture(const std::string& path);
    void applyToShader(ShaderProgram* shader);
    Material getMaterial() const { return material; }
    bool hasTextureMap() const { return hasTexture; }
    GLuint getTextureID() const { return textureID; }
};
