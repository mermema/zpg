#pragma once
#include "Model.h"
class ObjModel :
    public Model
{
public:
    ObjModel(const char* objFileName);
    void loadOBJ(const char* objFileName);

};

