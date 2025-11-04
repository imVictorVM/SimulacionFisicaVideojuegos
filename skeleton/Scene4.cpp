#include "Scene4.h"
#include "Particle.h"
#include "ParticleForceRegistry.h"
#include "GravityForceGenerator.h"
#include "ExplosionForceGenerator.h"
#include <iostream>

Scene4::Scene4() :
    _force_registry(nullptr),
    _gravity_generator(nullptr),
    _explosion_generator(nullptr)
{
}

Scene4::~Scene4() {
    cleanup();
}

void Scene4::initialize() {
    std::cout << "Inicializando " << getDescription() << std::endl;

    //Crear el registro y el generador de gravedad
    _force_registry = new ParticleForceRegistry();
    _gravity_generator = new GravityForceGenerator({ 0.0f, -9.8f, 0.0f });

    //Crear el generador de explosión
    _explosion_generator = new ExplosionForceGenerator(1000000.0f, 0.5f, 20.0f, { 0.0f, 1.0f, 0.0f });

    //Crear las partículas de prueba
    createParticleBox();
}

void Scene4::createParticleBox() {
    // Crea una cuadrícula de 5x5 partículas
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {

            Vector3 pos = { -10.0f + i * 5.0f, 1.0f, -10.0f + j * 5.0f };

            double mass = 1.0 + (i * j * 0.5);

            Particle* p = new Particle(pos, { 0.0f, 0.0f, 0.0f }, mass, 0.9, 100.0);

            float r = (float)(mass / 10.0);
            p->setColor({ r, 1.0f - r, 0.2f, 1.0f });
            p->setupVisual();

            _particles.push_back(p);

            _force_registry->add(p, _explosion_generator);
        }
    }
}

void Scene4::update(double t) {
    //1 Aplicar todas las fuerzas registradas (Explosión si está activa)
    _force_registry->updateForces(t);

    //2 Integrar las partículas
    auto it = _particles.begin();
    while (it != _particles.end()) {
        Particle* p = *it;
        if (!p->isAlive()) {
            _force_registry->remove(p, _explosion_generator);
            delete p;
            it = _particles.erase(it);
        }
        else {
            p->integrate(t);
            ++it;
        }
    }
}

void Scene4::cleanup() {
    for (auto p : _particles) {
        delete p;
    }
    _particles.clear();

    delete _force_registry;
    _force_registry = nullptr;
    delete _gravity_generator;
    _gravity_generator = nullptr;
    delete _explosion_generator;
    _explosion_generator = nullptr;
}

void Scene4::handleKeyPress(unsigned char key) {
    switch (toupper(key)) {
    case 'E': // Activar la explosión
    {
        _explosion_generator->trigger();
        break;
    }
    }
}