#include "Observable.h"
#include "Observer.h"

void Observable::registerObserver(Observer* observer) {
    observers.emplace_back(observer);
}

void Observable::removeObserver(Observer* observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void Observable::notifyObservers() {
    for (auto ptr : observers) {
        ptr->update(this);
    }
}
