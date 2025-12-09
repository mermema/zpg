#include "BezierScene.h"


void BezierScene::create()
{

    ShaderLoader vert("../zpg/Shaders/uv.vert", GL_VERTEX_SHADER);
    ShaderLoader frag("../zpg/Shaders/phongmultexture.frag", GL_FRAGMENT_SHADER);

    std::vector<Shader*> shaders = { &vert, &frag };


    ShaderProgram* textureshader = new ShaderProgram(shaders);

    ShaderLoader skyboxVert("../zpg/Shaders/skybox.vert", GL_VERTEX_SHADER);
    ShaderLoader skyboxFrag("../zpg/Shaders/skybox.frag", GL_FRAGMENT_SHADER);
    std::vector<Shader*> skyboxShaders = { &skyboxVert, &skyboxFrag };
    ShaderProgram* skyboxShader = new ShaderProgram(skyboxShaders);



    this->enableMouseActions();
    Camera* camera = new Camera(60.0f, 0.1f, 1000.0f);
    camera->setPosition(glm::vec3(0.0f, 8.0f, 20.0f));
    camera->setTarget(glm::vec3(0.0f, 0.1f, 0.0f));
    this->setCamera(camera);


    std::vector<std::string> skyboxFaces = {
"../zpg/Objects/sky/posx.jpg",   // +X
"../zpg/Objects/sky/negx.jpg",    // -X
"../zpg/Objects/sky/posy.jpg",    // +Y  
"../zpg/Objects/sky/negy.jpg",  // -Y
"../zpg/Objects/sky/posz.jpg",   // +Z
"../zpg/Objects/sky/negz.jpg",   // -Z
    };


    SkyBox* skybox = new SkyBox(skyboxShader, skyboxFaces);
    this->setSkyObject(skybox);


    auto dirlight = new DirectionalLight(glm::vec3(0, -1, -1));
    dirlight->setIntensity(1);
    //dirlight->setAttenuation(0, 0, 0);
    this->addLight(dirlight);

    Material woodMaterial(

        glm::vec3(0.1),
        glm::vec3(0, 1, 0),
        glm::vec3(0.1),
        32.0f
    );

    Material defaultMaterial;


    Texture* grassTexture = new Texture(woodMaterial, "../zpg/Objects/grass.png");


    ObjModel* terenModel = new ObjModel("../zpg/Objects/teren.obj");


    DrawableObject* terenObj = new DrawableObject(terenModel, textureshader, grassTexture);
    CompositeTransformation* plainTrans = new CompositeTransformation();
    plainTrans->add(new Scale(glm::vec3(4, 1, 4)));
    terenObj->setTransformation(plainTrans);
    terenObj->setObjectColor(glm::vec3(0, 1, 0));
    terenObj->setNotMovable();
    this->addObject(terenObj);




    Material formulaMaterial(glm::vec3(0.08), glm::vec3(1.0f), glm::vec3(0.8f), 16);

    Texture* formulaTexture = new Texture(formulaMaterial, "../zpg/Objects/wooden_fence.png");


    ObjModel* formule = new ObjModel("../zpg/Objects/formula1.obj");
    DrawableObject* formulaObject = new DrawableObject(formule, textureshader, formulaTexture);
    CompositeTransformation* formulaTrans = new CompositeTransformation();
    formulaTrans->add(new Scale(glm::vec3(0.3)));
    formulaTrans->add(new Rotation(glm::radians(90.0), glm::vec3(0, 1, 0)));
    formulaTrans->add(this->splineTransformation);
    formulaObject->setTransformation(formulaTrans);
    this->splineObjectTransformations = formulaTrans;
    formulaObject->setObjectColor(glm::vec3(0, 1, 0));
    this->addObject(formulaObject);



    camera->registerObserver(textureshader);

    dirlight->registerObserver(textureshader);
    camera->notifyObservers();
    dirlight->notifyObservers();




}

void BezierScene::handleMouseClick(double x, double y)
{
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

        cout << "Clicked on pixel: " << (int)x << ", " << (int)y << ", " << depth << " stencil index " << stencilIndex << endl;


        //save stencilid 
        selectedObjectID = stencilIndex;

        //compute worldpos of clicl
        glm::vec3 screenPos = glm::vec3(x, glY, depth);
        glm::vec4 viewportVec = glm::vec4(viewport[0], viewport[1], viewport[2], viewport[3]);

        glm::mat4 view = camera->getViewMatrix();
        glm::mat4 projection = camera->getProjectionMatrix();

        glm::vec3 lastClickedWorldPos = glm::unProject(screenPos, view, projection, viewportVec);


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


        this->points.push_back(lastClickedWorldPos);
    }

void BezierScene::handlePKey()
{
    this->splineObjectTransformations->remove(splineTransformation);
    splineTransformation = new DynamicBezierSpline(points, 0.1);
    this->splineObjectTransformations->add(splineTransformation);
    this->points.clear();
}
