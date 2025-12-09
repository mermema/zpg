#pragma once
#include "DrawableObject.h"
#include <vector>
#include "Light.h"
#include "FlashLight.h"
#include "SkyObject.h"

class Scene {
public:
    Scene();
    ~Scene();

    void addObject(DrawableObject* obj);
    void addLight(Light* light);
    virtual void draw();
    const std::vector<DrawableObject*>& getObjects() const { return objects; }
    void setCamera(Camera* cam) { camera = cam; }
    Camera* getCamera() { return camera; }
    void activate();
    //void createLight(const glm::vec3& position, const glm::vec3& color, float intensity);
    void registerShaderToAllLightsAndNotify(ShaderProgram* shader);
    void registerAllObjectsToCameraAndNotify();
    void registerAllShadersToAllLightsAndNotify();
    bool hasFlashlight() const { return flashlight != nullptr; }
    Flashlight* getFlashlight() const { return flashlight; }
    void enableFlashlight();
    void disableFlashlight();
    void toggleFlashlight();

    void setSkyObject(SkyObject* sky) { skyObject = sky; }
    SkyObject* getSkyObject() const { return skyObject; }
    void drawSky();

    void enableStencilTest();
    void disableStencilTest();
    virtual void handleMouseClick(double x, double y);
    virtual void handlePKey() {};
    virtual void handleMouseRelease(double x, double y);
    unsigned int getSelectedObjectID() const { return selectedObjectID; }
    glm::vec3 getLastClickedWorldPos() const { return lastClickedWorldPos; }

    DrawableObject* findObjectByID(unsigned int id) const;
    void addObjectAtPosition(DrawableObject* obj, glm::vec3& position);

    void setClickCreateModel(Model* model) { clickCreateModel = model; }
    void setClickCreateShader(ShaderProgram* shader) { clickCreateShader = shader; }
    void setClickCreateTexture(Texture* texture) { clickCreateTexture = texture; }

    void enableMouseActions() { isMouseSensitive = true; }

private:
    std::vector<DrawableObject*> objects;
    std::vector<Light*> lights;
    Flashlight* flashlight = nullptr;
    SkyObject* skyObject = nullptr;




protected:
    glm::vec3 lastClickedWorldPos;
    void removeObjectByID(int id);
    Camera* camera = nullptr;
    unsigned int selectedObjectID;
    bool isMouseSensitive = false;

    //moving objects
    DrawableObject* pickedObject = nullptr;
    void pickUpObject(unsigned int objectID);
    void placeObject(const glm::vec3& worldPos);

    //creating object by mouse
    Model* clickCreateModel = nullptr;
    ShaderProgram* clickCreateShader = nullptr;
    Texture* clickCreateTexture = nullptr;
    DrawableObject* clickCreateObject();

};
