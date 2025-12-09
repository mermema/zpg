#pragma once
#include "Scene.h"
#include "Rotation.h"
#include "Scale.h"
#include "MatrixTransformation.h"
#include "DirectionaLight.h"
#include "ObjModel.h"
#include "Models/plain.h"
#include "ShaderLoader.h"
#include "SkyBox.h"
#include "Models/sphere.h"
#include "DynamicTranslation.h"
#include "DynamicTranslationTwoSegment.h"
#include "SkyDome.h"
class GameScene :
    public Scene
{
public:
    void create();
    void draw() override;
    void handleMouseClick(double x, double y) override;

private:
    DrawableObject* activeObject = nullptr;
    float lastSpawnTime = 0.0;
    float spawnInterval = 0.0;
    int score = 0;

    ShaderProgram* textureshader;

    std::vector<ObjModel*> models;
    std::vector<Texture*> textures;
    DrawableObject* createObjectByIndex(int index);
    void spawnRandomObject();


};

