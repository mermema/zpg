#include "Scene.h"
#include <GLFW/glfw3.h>

Scene::Scene() {}

Scene::~Scene() {
    for (auto obj : objects) {
        delete obj;
    }
}

void Scene::addObject(DrawableObject* obj) {
    objects.push_back(obj);
}

void Scene::draw() {
    for (auto obj : objects) {
        obj->draw();
    }
}
void Scene::draw(Camera* camera) {
    for (auto obj : objects) {
        obj->draw(camera); // <- pøedáváme kameru dál
    }
}

void Scene::draw(const glm::mat4& view, const glm::mat4& projection) {
    for (auto obj : objects) {
        obj->draw(view, projection);
    }
}


void Scene::update() {
    for (auto obj : objects) {
        obj->update((float)glfwGetTime());
    }
}
