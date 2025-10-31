#include "Scene1.h"
#include "RenderUtils.hpp"
#include "ParticleForceRegistry.h"
#include "GravityForceGenerator.h"

#include "Bullet.h"
#include "CannonBall.h"
#include "TankBall.h"
#include "LaserPistol.h"

#include <iostream>

Scene1::Scene1() :
    _force_registry(nullptr),
    _gravity_generator(nullptr)
{
}

Scene1::~Scene1() {
    cleanup();
}

void Scene1::initialize() {
    std::cout << "Inicializando " << getDescription() << std::endl;

    _force_registry = new ParticleForceRegistry();
    _gravity_generator = new GravityForceGenerator({ 0.0, -9.8, 0.0 });
}

void Scene1::update(double t) {
    //1 Aplicar fuerzas a todas las partículas registradas
    _force_registry->updateForces(t);

    //2 Integrar todos los proyectiles y eliminar los que han muerto
    auto it = _projectiles.begin();
    while (it != _projectiles.end()) {
        Projectile* p = *it;
        if (!p->isAlive()) {
            _force_registry->remove(p, _gravity_generator); //Eliminar del registro de fuerzas
            delete p;
            it = _projectiles.erase(it);
        }
        else {
            p->integrate(t);
            ++it;
        }
    }
}

void Scene1::cleanup() {
    for (auto p : _projectiles) {
        delete p;
    }
    _projectiles.clear();

    delete _force_registry;
    _force_registry = nullptr;
    delete _gravity_generator;
    _gravity_generator = nullptr;
}

void Scene1::handleKeyPress(unsigned char key) {
    switch (toupper(key)) {
    case 'B': createProjectile(1); break;
    case 'C': createProjectile(2); break;
    case 'T': createProjectile(3); break;
    case 'L': createProjectile(4); break;
    case ' ':
        for (auto p : _projectiles) {
            _force_registry->remove(p, _gravity_generator);
            delete p;
        }
        _projectiles.clear();
        break;
    }
}

void Scene1::createProjectile(int type) {
    Camera* cam = GetCamera();
    Vector3 startPos = cam->getEye();
    Vector3 direction = cam->getDir();
    Projectile* p = nullptr;

    switch (type) {
    case 1: p = new Bullet(startPos, direction * 80.0f); break;
    case 2: p = new CannonBall(startPos, direction * 40.0f); break;
    case 3: p = new TankBall(startPos, direction * 150.0f); break;
    case 4: p = new LaserPistol(startPos, direction * 250.0f); break;
    }

    if (p) {
        p->setupVisual();
        _projectiles.push_back(p);
        //Registramos la nueva partícula para que sea afectada por la gravedad
        _force_registry->add(p, _gravity_generator);
    }
}