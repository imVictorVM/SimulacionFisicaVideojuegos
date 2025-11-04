#include "Scene1.h"
#include "RenderUtils.hpp"
#include "ParticleForceRegistry.h"
#include "GravityForceGenerator.h"
#include "Render/Camera.h"

#include "Bullet.h"
#include "CannonBall.h"
#include "TankBall.h"
#include "LaserPistol.h"

#include <iostream>

Scene1::Scene1() :
    _force_registry(nullptr),
    _standard_gravity(nullptr),
    _bullet_gravity(nullptr)
{
}

Scene1::~Scene1() {
    cleanup();
}

void Scene1::initialize() {
    std::cout << "Inicializando " << getDescription() << std::endl;

    _force_registry = new ParticleForceRegistry();

    //1 Gravedad estándar (para proyectiles pesados)
    _standard_gravity = new GravityForceGenerator({ 0.0f, -9.8f, 0.0f });

    //2 Gravedad de la bala
    //g_sim = g_real * (v_sim / v_real)^2
    //g_sim = 9.8 * (80 / 850)^2 = 9.8 * 0.0088 = 0.086
    //Una gravedad casi nula, para una trayectoria casi recta.
    _bullet_gravity = new GravityForceGenerator({ 0.0f, -0.086f, 0.0f });
}

void Scene1::update(double t) {
    //1 Aplicar fuerzas
    _force_registry->updateForces(t);

    //2 Integrar y limpiar
    auto it = _projectiles.begin();
    while (it != _projectiles.end()) {
        Projectile* p = *it;
        if (!p->isAlive()) {
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
   
    if (!_force_registry) return;
    
    for (auto p : _projectiles) {
        delete p;
    }
    _projectiles.clear();

    _force_registry->clear();
    delete _force_registry;
    _force_registry = nullptr;

    delete _standard_gravity;
    _standard_gravity = nullptr;
    delete _bullet_gravity;
    _bullet_gravity = nullptr;
}

void Scene1::handleKeyPress(unsigned char key) {
    switch (toupper(key)) {
    case 'B': createProjectile(1); break;
    case 'C': createProjectile(2); break;
    case 'T': createProjectile(3); break;
    case 'L': createProjectile(4); break;
    case ' ':
        cleanup();
        initialize();
        std::cout << "Escena 1 limpiada y reiniciada." << std::endl;
        break;
    }
}

void Scene1::createProjectile(int type) {
    Camera* cam = GetCamera();
    Vector3 startPos = cam->getEye();
    Vector3 direction = cam->getDir();
    Projectile* p = nullptr;

    switch (type) {
    case 1: // Bullet
        p = new Bullet(startPos, direction * 80.0f);
        if (p) {
            _projectiles.push_back(p);
            _force_registry->add(p, _bullet_gravity);
        }
        break;
    case 2: // CannonBall
        p = new CannonBall(startPos, direction * 40.0f);
        if (p) {
            _projectiles.push_back(p);
            _force_registry->add(p, _standard_gravity);
        }
        break;
    case 3: // TankBall
        p = new TankBall(startPos, direction * 150.0f);
        if (p) {
            _projectiles.push_back(p);
            _force_registry->add(p, _standard_gravity);
        }
        break;
    case 4: // LaserPistol
        p = new LaserPistol(startPos, direction * 250.0f);
        if (p) {
            _projectiles.push_back(p);
        }
        break;
    }

    if (p) {
        p->setupVisual();
    }
}