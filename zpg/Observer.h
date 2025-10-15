#pragma once
#include <glm/mat4x4.hpp>

class Observable;

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(Observable* who) = 0;
};
