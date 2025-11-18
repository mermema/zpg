#pragma once
#include "Texture.h"
#include "ShaderProgram.h"
#include "SkyObject.h"
#include <vector>
#include <string>

class SkyBox : public Texture, public SkyObject {
private:
    GLuint VAO, VBO;
    ShaderProgram* shader;

public:
    SkyBox(ShaderProgram* shaderProgram, const std::vector<std::string>& faces);
    ~SkyBox();

    void loadCubemap(const std::vector<std::string>& faces);
    void applyToShader(ShaderProgram* shader);

    void draw(const glm::mat4& view, const glm::mat4& projection) override;
    ShaderProgram* getShader() const override { return shader; }

private:
    void setupSkyboxVAO();
};