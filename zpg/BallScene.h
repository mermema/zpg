#pragma once
#include "Scene.h"
#include "Scale.h"
#include "Translation.h"
#include "Models/sphere.h"
#include "ShaderLoader.h"
class BallScene :
    public Scene
{
public:
    void create();
private:
    Model sph1;


};
