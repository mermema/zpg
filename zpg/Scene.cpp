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

void Scene::update() {
    for (auto obj : objects) {
        obj->update((float)glfwGetTime());
    }
}
