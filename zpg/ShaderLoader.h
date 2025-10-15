#pragma once
#include "Shader.h"
#include <string>
#include <GL/glew.h>
#include <iostream>)
#include <fstream>
#include <sstream>


class ShaderLoader : public Shader
{
public:
    ShaderLoader(const std::string& path, GLenum type);
    ~ShaderLoader();
private:
    static std::string loadFile(const std::string& path);
};
