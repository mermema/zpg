#pragma once
#include "Scene.h"
#include "FlashLight.h"
#include "DirectionaLight.h"
#include "RandomTranslation.h"
#include "Scale.h"
#include "Translation.h"
#include "Firefly.h"
#include "Models/bushes.h"
#include "Models/tree.h"
#include "Models/plain.h"
#include "Models/sphere.h"
#include "ShaderLoader.h"
class ForrestScene :
    public Scene
{
public:
    void create();
};

