#include "Scene.h"

using namespace physx;
using namespace std;


void Scene::cleanup() {
    
}

void Scene::update(double t) {


    // Actualizar proyectiles y limpiar los muertos
    for (auto it = projectiles.begin(); it != projectiles.end(); ) {
        if (!(*it)->isAlive()) {
            delete* it;
            it = projectiles.erase(it);
        }
        else {
            (*it)->integrate(t);
            ++it;
        }
    }
}