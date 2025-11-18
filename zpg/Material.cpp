#include "Material.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool Material::loadFromMTL(const std::string& filepath) {
    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Failed to open MTL file: " << filepath << std::endl;
        return false;
    }

    std::string line;
    bool foundAnyData = false;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "Ns") {
            float ns;
            iss >> ns;
            shininess = glm::clamp((ns / 1000.0f) * 128.0f, 1.0f, 128.0f);
            foundAnyData = true;
        }
        else if (prefix == "Ka") {
            float r, g, b;
            iss >> r >> g >> b;
            ambient = glm::clamp(glm::vec3(r, g, b), 0.0f, 1.0f) * 0.1f;
            foundAnyData = true;
        }
        else if (prefix == "Kd") {
            float r, g, b;
            iss >> r >> g >> b;
            diffuse = glm::vec3(r, g, b);
            foundAnyData = true;
        }
        else if (prefix == "Ks") {
            float r, g, b;
            iss >> r >> g >> b;
            specular = glm::vec3(r, g, b);
            foundAnyData = true;
        }
    }

    if (foundAnyData) {
        std::cout << "Loaded material values from: " << filepath << std::endl;
        return true;
    }

    std::cout << "No material data found in: " << filepath << std::endl;
    return false;
}