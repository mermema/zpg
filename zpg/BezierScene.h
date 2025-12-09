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
#include "DynamicBezierSpline.h"
#include "DynamicBezierSplineCS.h"

class BezierScene :
    public Scene
{
public:
    void create();
    void handleMouseClick(double x, double y) override;
    void handlePKey() override;
private:
    std::vector<glm::vec3> points;
    CompositeTransformation* splineObjectTransformations;
    BasicTransformation* splineTransformation = new MatrixTransformation(glm::mat4(1));


};

