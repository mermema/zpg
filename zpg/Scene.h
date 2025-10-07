#pragma once
#include "DrawableObject.h"
#include <vector>

class Scene {
public:
    Scene();
    ~Scene();

    void addObject(DrawableObject* obj);
    void draw();
    void draw(Camera* camera);
    void draw(const glm::mat4& view, const glm::mat4& projection);
    void update();
    const std::vector<DrawableObject*>& getObjects() const { return objects; }


private:
    std::vector<DrawableObject*> objects;
};
