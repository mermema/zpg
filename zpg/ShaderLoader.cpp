#include "ShaderLoader.h"

std::string ShaderLoader::loadFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: cant open shader file! file path:  " << path << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}

ShaderLoader::ShaderLoader(const std::string& path, GLenum type)
    : Shader( loadFile(path), type)
{
}

ShaderLoader::~ShaderLoader() = default;
