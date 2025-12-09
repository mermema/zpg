#include "ObjectScene.h"
#include "ObjModel.h"
#include "DirectionaLight.h"
#include "ReflectorLight.h"

void ObjectScene::create()
{
        Camera* camera = new Camera();
        camera->setPosition(glm::vec3(0, 1, 5));
        Light* light = new ReflectorLight(glm::vec3(0, 5, 0), glm::vec3(0, -1, 0));
        Light* directiional = new DirectionalLight(glm::vec3(0, -1, 0));
        light->setIntensity(0.5);
        light->setAttenuation(0.01, 0.01, 0.01);
        this->setCamera(camera);
        this->addLight(light);
        this->addLight(directiional);
        static ShaderLoader frag("../zpg/Shaders/uni.vert", GL_VERTEX_SHADER);
        static ShaderLoader constant("../zpg/Shaders/coloruni/phongmulti.frag", GL_FRAGMENT_SHADER);
        std::vector<Shader*> sh = { &frag, &constant };
        static ShaderProgram shaderPrg(sh);

        static ObjModel model("../zpg/Objects/formula1.obj");
        static Model planemodel;
        planemodel.upload(plain, sizeof(plain) / (6 * sizeof(float)));
        DrawableObject* planeobj = new DrawableObject(&planemodel, &shaderPrg);
        planeobj->setObjectColor(glm::vec3(0.2, 0.8, 0.2));
        CompositeTransformation* planetransformation = new CompositeTransformation();
        planetransformation->add(new Scale(glm::vec3(10, 1, 10)));
        planeobj->setTransformation(planetransformation);

        //
        static ObjModel cat("../zpg/Objects/cat.obj");
        DrawableObject* catObject = new DrawableObject(&cat, &shaderPrg);
        catObject->setObjectColor(glm::vec3(0.8, 0.5,0.5));
        CompositeTransformation* catTrasform = new CompositeTransformation();
        catTrasform->add(new Rotation(-3.1415926 / 2, glm::vec3(1, 0, 0)));
        catTrasform->add(new Scale(glm::vec3(0.015)));
        catTrasform->add(new Translation(glm::vec3(-3, 0, -1)));
        catObject->setTransformation(catTrasform);
        this->addObject(catObject);


        //plane
        static ObjModel airplane("../zpg/Objects/airplane.obj");
        DrawableObject* airplaneObject = new DrawableObject(&airplane, &shaderPrg);
        airplaneObject->setObjectColor(glm::vec3(0.5, 0.5, 1));
        CompositeTransformation* airplaneTrasform = new CompositeTransformation();
        airplaneTrasform->add(new Rotation(-3.1415926 / 2, glm::vec3(1, 0, 0)));
        airplaneTrasform->add(new Scale(glm::vec3(0.001)));
        airplaneTrasform->add(new Translation(glm::vec3(0,1, -6)));
        airplaneObject->setTransformation(airplaneTrasform);
        this->addObject(airplaneObject);
        this->addObject(planeobj);


        //sofa     
        static ObjModel sofa("../zpg/Objects/Sofa.obj");
        DrawableObject* sofaObject = new DrawableObject(&sofa, &shaderPrg);
        sofaObject->setObjectColor(glm::vec3(0, 1, 1));
        CompositeTransformation* sofaTrasform = new CompositeTransformation();
        sofaTrasform->add(new Rotation(-3.1415926 / 2, glm::vec3(0, 1, 0)));
        sofaTrasform->add(new Scale(glm::vec3(0.1)));
        sofaTrasform->add(new Translation(glm::vec3(3, -0.010, 0)));
        sofaObject->setTransformation(sofaTrasform);
        this->addObject(sofaObject);

        
        //formule 
        DrawableObject* formule = new DrawableObject(&model, &shaderPrg);
        formule->setObjectColor(glm::vec3(0.5));
        CompositeTransformation* transformation = new CompositeTransformation();
        transformation->add(new Scale(glm::vec3(0.05)));
        transformation->add(new DynamicRotation(glm::vec3(0, 1, 0), 0.1));

        formule->setTransformation(transformation);
        this->addObject(formule);
        camera->registerObserver(&shaderPrg);
        light->registerObserver(&shaderPrg);
        light->notifyObservers();
        directiional->registerObserver(&shaderPrg);
        directiional->notifyObservers();


        camera->notifyObservers();


}
