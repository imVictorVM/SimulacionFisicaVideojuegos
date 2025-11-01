#include "Scene3.h"
#include "Render/Camera.h"
#include "Bullet.h"
#include "ParticleForceRegistry.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"
#include <iostream>

Scene3::Scene3() :
    _force_registry(nullptr),
    _gravity_generator(nullptr),
    _wind_generator(nullptr)
{
}

Scene3::~Scene3() {
    cleanup();
}

void Scene3::initialize() {
    std::cout << "Inicializando " << getDescription() << std::endl;

    //Crear el registro y los generadores de fuerza
    _force_registry = new ParticleForceRegistry();
    _gravity_generator = new GravityForceGenerator({ 0.0, -9.8, 0.0 });

    //Viento que sopla de izquierda a derecha
    _wind_generator = new WindForceGenerator({ 20.0, 0.0, 0.0 });
    //Definimos un "túnel de viento" en el centro de la escena
    _wind_generator->setWindArea({ -10, 0, -10 }, { 10, 10, 10 });

}

void Scene3::update(double t) {
    //Aplicar todas las fuerzas a las partículas registradas
    _force_registry->updateForces(t);

    //Integrar las partículas y eliminar las que han muerto
    auto it = _particles.begin();
    while (it != _particles.end()) {
        Particle* p = *it;
        if (!p->isAlive()) {
            //Eliminar la partícula de todos los registros de fuerza
            _force_registry->remove(p, _gravity_generator);
            _force_registry->remove(p, _wind_generator);
            delete p;
            it = _particles.erase(it);
        }
        else {
            p->integrate(t);
            ++it;
        }
    }
}

void Scene3::cleanup() {
    for (auto p : _particles) {
        delete p;
    }
    _particles.clear();

    delete _force_registry;
    _force_registry = nullptr;
    delete _gravity_generator;
    _gravity_generator = nullptr;
    delete _wind_generator;
    _wind_generator = nullptr;
}

void Scene3::handleKeyPress(unsigned char key) {
    switch (toupper(key)) {
    case 'B':
    {
        Camera* cam = GetCamera();
        Bullet* p = new Bullet(cam->getEye(), cam->getDir() * 80.0f);
        p->setupVisual();
        _particles.push_back(p);

        
        _force_registry->add(p, _gravity_generator);
        _force_registry->add(p, _wind_generator);

        std::cout << "Bala disparada en la zona de viento." << std::endl;
        break;
    }
    }
}