#pragma once
#include <vector>
#include "Observer.h"

class Observable {
protected:
    std::vector<Observer*> observers;

public:
    void registerObserver(Observer* observer);
    void removeObserver(Observer* observer);
    virtual void notifyObservers();
};
