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
class ShaderTestScene :
    public Scene
{
public:
    void create();
};
