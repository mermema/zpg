#include "Scene.h"
#include <GLFW/glfw3.h>
#include <unordered_set>
#include "ObjModel.h"

Scene::Scene() {}

Scene::~Scene() {
    for (auto obj : objects) {
        delete obj;
    }
    for (auto light : lights) {
        delete light;
    }
}

void Scene::addObject(DrawableObject* obj) {
    objects.push_back(obj);
}

void Scene::drawSky() {
    if (!skyObject || !camera) return;

    //seve actual depth
    GLint oldDepthFunc;
    glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFunc);

    //set depth to draw 
    glDepthFunc(GL_LEQUAL);

    //actual draw
    skyObject->draw(camera->getViewMatrix(), camera->getProjectionMatrix());

    //set back to saved depth state
    glDepthFunc(oldDepthFunc);
    glClear(GL_DEPTH_BUFFER_BIT);
}


void Scene::draw() {
    if (skyObject) {
        drawSky();
    }


    if (flashlight) {
        flashlight->notifyObservers();//not really notify - overrided method which is notifying only if camera pos changes....
    }

    for (auto light : lights) {
        if (light->hasDynamic()) {
            light->notifyObservers();
        }
    }

    for (auto obj : objects) {
        ShaderProgram* shader = obj->getShader();
        if (shader) {
            shader->use();
            shader->setNumberOfLights((int)lights.size());
            shader->unset();
        }
    }

    enableStencilTest();

    for (auto obj : objects) {
        glStencilFunc(GL_ALWAYS, obj->getID(), 0xFF);
        obj->draw();
    }
    disableStencilTest();

}

void Scene::handleMouseClick(double x, double y) {
    if (!camera || !isMouseSensitive) return;

    GLint viewport[4];
    GLfloat depth;
    GLuint stencilIndex;

    glGetIntegerv(GL_VIEWPORT, viewport);

    //revers y axis 
    GLint glY = viewport[3] - (GLint)y - 1;

    //load into buffers
    glReadPixels((GLint)x, glY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    glReadPixels((GLint)x, glY, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &stencilIndex);

    cout<<"Clicked on pixel: " << (int)x<<", " << (int)y << ", " << depth << " stencil index " << stencilIndex << endl;
         

    //save stencilid 
    selectedObjectID = stencilIndex;

    //compute worldpos of clicl
    glm::vec3 screenPos = glm::vec3(x, glY, depth);
    glm::vec4 viewportVec = glm::vec4(viewport[0], viewport[1], viewport[2], viewport[3]);

    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 projection = camera->getProjectionMatrix();

    lastClickedWorldPos = glm::unProject(screenPos, view, projection, viewportVec);


    //changing final pos id is clicked to far....
    if (500 < std::max(std::abs(lastClickedWorldPos.x), std::max(std::abs(lastClickedWorldPos.y), std::abs(lastClickedWorldPos.z))))
    {
        lastClickedWorldPos /= 10;
    }

    /* //REMOVING object by click

            removeObjectByID(stencilIndex);
    */
    // ADDING object
    //DrawableObject* newObj = clickCreateObject();
    //addObjectAtPosition(newObj, lastClickedWorldPos);
    

    pickUpObject(stencilIndex);
}

void Scene::handleMouseRelease(double x, double y) {


    if (!camera || !isMouseSensitive) return;

    GLint viewport[4];
    GLfloat depth;
    GLuint stencilIndex;

    glGetIntegerv(GL_VIEWPORT, viewport);

    GLint glY = viewport[3] - (GLint)y - 1;

    glReadPixels((GLint)x, glY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    glReadPixels((GLint)x, glY, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &stencilIndex);


    selectedObjectID = stencilIndex;

    glm::vec3 screenPos = glm::vec3(x, glY, depth);
    glm::vec4 viewportVec = glm::vec4(viewport[0], viewport[1], viewport[2], viewport[3]);

    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 projection = camera->getProjectionMatrix();

    lastClickedWorldPos = glm::unProject(screenPos, view, projection, viewportVec);


    if (500 < std::max(std::abs(lastClickedWorldPos.x),std::max(std::abs(lastClickedWorldPos.y),std::abs(lastClickedWorldPos.z))))
    {
        lastClickedWorldPos /= 10;
    }


    if (pickedObject != nullptr) {
       placeObject(lastClickedWorldPos);
    }

   }

void Scene::pickUpObject(unsigned int objectID) {
    for (size_t i = 0; i < objects.size(); i++) {
        if (objects[i]->getID() == objectID) {
            if(objects[i]->getNotMovable())
            {
                return;
            }
            pickedObject = objects[i];
            objects.erase(objects.begin() + i);
            return;
        }
    }
}

void Scene::placeObject(const glm::vec3& worldPos) {
    if (pickedObject) {
       /* if (pickedObject->getNotMovable())
        {
            return;
        }*/  //REDUNDANT in case it is called only after pickUpObject
        if (!pickedObject->getTransformation()) {
            pickedObject->setTransformation(new CompositeTransformation());
        }
         glm::mat4 modelMatrix = pickedObject->getTransformation()->getMatrix();
         glm::vec3 currentPos = glm::vec3(modelMatrix[3]); 

         glm::vec3 delta = worldPos - currentPos;

         CompositeTransformation* trans = pickedObject->getTransformation();
         trans->add(new Translation(delta));

         objects.push_back(pickedObject);


        pickedObject = nullptr;
    }
}

void Scene::removeObjectByID( int id) {
    for (size_t i = 0; i < objects.size(); i++) {
        if (objects[i]->getID() == id) {
            if (objects[i]->getNotMovable())
            {
                return;
            }
            delete objects[i]; 
            objects.erase(objects.begin() + i); //earsing from scenes in this objects
            //std::cout<<"removed object with id:" << id << endl;
            return;
        }
    }
    std::cout << "Object wuth that id not found id: " << id << endl;

}



void Scene::enableStencilTest() {
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

void Scene::disableStencilTest() {
    glDisable(GL_STENCIL_TEST);
}

DrawableObject* Scene::findObjectByID(unsigned int id) const {
    for (auto obj : objects) {
        if (obj->getID() == id) {
            return obj;
        }
    }
    return nullptr;
}

void Scene::addObjectAtPosition(DrawableObject* obj, glm::vec3& position) {
    if (!obj)
    {
        return;
    }
    CompositeTransformation* transform = new CompositeTransformation();
    transform->add(new Translation(position));
    obj->setTransformation(transform);

    addObject(obj);

    cout << "Added object with ID: " << obj->getID() << endl;

}

DrawableObject* Scene::clickCreateObject() {
    if(!clickCreateModel || !clickCreateShader)
    {
        return nullptr;
    }

    DrawableObject* newObj = nullptr;

    if (dynamic_cast<ObjModel*>(clickCreateModel) && clickCreateTexture != nullptr)
    {
        newObj = new DrawableObject(clickCreateModel, clickCreateShader, clickCreateTexture);
    }
    else
    {
        newObj = new DrawableObject(clickCreateModel, clickCreateShader);
    }
    newObj->setObjectColor(glm::vec3(1,1,0));

    return newObj;
}


void Scene::activate() {
    if (camera) {
        camera->calculateViewMatrix();
        camera->notifyObservers();
    }

    for (auto light : lights) {
        light->notifyObservers();
    }

}

void Scene::addLight(Light* light) {
    int index = (int)lights.size();
    light->setIndex(index);
    lights.push_back(light);

    Flashlight* flash = dynamic_cast<Flashlight*>(light);
    if (flash) {
        flashlight = flash;
    }

}

void Scene::registerShaderToAllLightsAndNotify(ShaderProgram* shader) {

    for (auto light : lights) {
        light->registerObserver(shader);
        light->notifyObservers();
    }
}

void Scene::registerAllShadersToAllLightsAndNotify() {
    std::unordered_set<ShaderProgram*> registeredShaders;

    for (auto obj : objects) {
        ShaderProgram* shader = obj->getShader();
        if (shader && registeredShaders.find(shader) == registeredShaders.end()) {
            for (auto light : lights) {
                light->registerObserver(shader);
            }
            registeredShaders.insert(shader);
        }
    }

    for (auto light : lights) {
        light->notifyObservers();
    }
}


void Scene::registerAllObjectsToCameraAndNotify() {
    std::unordered_set<ShaderProgram*> registeredShaders;

    for (auto obj : objects) {
        ShaderProgram* shader = obj->getShader();
        if (shader && registeredShaders.find(shader) == registeredShaders.end()) {
            camera->registerObserver(shader);
            registeredShaders.insert(shader);
        }
    }
    camera->notifyObservers();
}


void Scene::enableFlashlight() {
    if (flashlight) {
        flashlight->enable();
    }
}

void Scene::disableFlashlight() {
    if (flashlight) {
        flashlight->disable();
    }
}

void Scene::toggleFlashlight() {
    if (flashlight) {
        flashlight->toggle();
    }
}