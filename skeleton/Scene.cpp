#include "Scene.h"

using namespace physx;
using namespace std;


void Scene::cleanup() {
    for (auto p : particles) {
        delete p;
    }
    particles.clear();

    for (auto p : projectiles) {
        delete p;
    }
    projectiles.clear();
}

void Scene::update(double t) {
    for (auto p : particles) {
        p->integrate(t);
    }

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