#pragma once
#include "Scene.h"
#include "ShaderLoader.h"
#include "SkyBox.h"
#include "DirectionaLight.h"
#include "ObjModel.h"
#include "Rotation.h"
#include "Scale.h"
#include "Translation.h"
#include "Models/sphere.h"
class ObjectPlaceScene :
    public Scene
{
public:
    void create();
    void handleMouseClick(double x, double y) override;
    void handleMouseRelease(double x, double y) override {};


};

