#include "Scene3.h"
#include "RenderUtils.hpp"
#include "ParticleForceRegistry.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"
#include "WhirlwindForceGenerator.h"

#include "Bullet.h"
#include "CannonBall.h"
#include "TankBall.h"
#include "LaserPistol.h"


#include <iostream>

using namespace physx;

Scene3::Scene3() :
    _force_registry(nullptr),
    _gravity_generator(nullptr),
    _wind_generator(nullptr),
    _whirlwind_generator(nullptr),
    _wind_area_visual(nullptr),
    _whirlwind_area_visual(nullptr)
{
}

Scene3::~Scene3() {
    cleanup();
}

void Scene3::initialize() {
    std::cout << "Inicializando " << getDescription() << std::endl;

    //Registro de fuerzas
    _force_registry = new ParticleForceRegistry();

    //Gravedad
    _gravity_generator = new GravityForceGenerator({ 0.0f, -9.8f, 0.0f });

    //Viento
    _wind_generator = new WindForceGenerator({ 50.0f, 0.0f, 0.0f });
    Vector3 windMin = { -25.0f, 0.0f, 0.0f };
    Vector3 windMax = { -5.0f, 20.0f, 30.0f };
    _wind_generator->setWindArea(windMin, windMax);

    //Torbellino
    _whirlwind_generator = new WhirlwindForceGenerator(
        150.0f,                             // intensidad
        { 15.0f, 0.0f, 15.0f },             // centro
        { 0.0f, 1.0f, 0.0f }                // eje vertical
    );
    Vector3 whirlMin = { 5.0f, 0.0f, 0.0f };
    Vector3 whirlMax = { 25.0f, 20.0f, 30.0f };
    _whirlwind_generator->setWindArea(whirlMin, whirlMax);

    //Visualización de las zonas
  //  createWindZoneVisual(windMin, windMax, { 0.2f, 0.5f, 1.0f, 0.3f }, false);
  //  createWindZoneVisual(whirlMin, whirlMax, { 1.0f, 0.5f, 0.1f, 0.3f }, true);
}
/*
void Scene3::createWindZoneVisual(const Vector3& min, const Vector3& max, const Vector4& color, bool isWhirlwind) {
    Vector3 center = (min + max) * 0.5f;
    Vector3 halfSize = (max - min) * 0.5f;

    PxShape* boxShape = CreateShape(PxBoxGeometry(halfSize.x, halfSize.y, halfSize.z));
    PxTransform* transform = new PxTransform(center);

    RenderItem* visual = new RenderItem(boxShape, transform, color);
    RegisterRenderItem(visual);

    if (isWhirlwind)
        _whirlwind_area_visual = visual;
    else
        _wind_area_visual = visual;
}
*/
void Scene3::update(double t) {
    _force_registry->updateForces(t);

    auto it = _projectiles.begin();
    while (it != _projectiles.end()) {
        Projectile* p = *it;
        if (!p->isAlive()) {
            _force_registry->remove(p, _gravity_generator);
            _force_registry->remove(p, _wind_generator);
            _force_registry->remove(p, _whirlwind_generator);
            delete p;
            it = _projectiles.erase(it);
        }
        else {
            p->integrate(t);
            ++it;
        }
    }
}

void Scene3::cleanup() {
    
    if (!_force_registry) return;
    // Eliminar proyectiles
    for (auto p : _projectiles) {
        _force_registry->remove(p, _gravity_generator);
        _force_registry->remove(p, _wind_generator);
        _force_registry->remove(p, _whirlwind_generator);
        delete p;
    }
    _projectiles.clear();

    // Eliminar visuales de zonas
    if (_wind_area_visual) {
        DeregisterRenderItem(_wind_area_visual);
        delete _wind_area_visual;
        _wind_area_visual = nullptr;
    }
    if (_whirlwind_area_visual) {
        DeregisterRenderItem(_whirlwind_area_visual);
        delete _whirlwind_area_visual;
        _whirlwind_area_visual = nullptr;
    }

    // Eliminar generadores y registro
    delete _force_registry; _force_registry = nullptr;
    delete _gravity_generator; _gravity_generator = nullptr;
    delete _wind_generator; _wind_generator = nullptr;
    delete _whirlwind_generator; _whirlwind_generator = nullptr;
}

void Scene3::handleKeyPress(unsigned char key) {
    switch (toupper(key)) {
    case 'B': createProjectile(1); break;
    case 'C': createProjectile(2); break;
    case 'T': createProjectile(3); break;
    case 'L': createProjectile(4); break;

    case ' ':
        break;
    default:
        break;
    }
}

void Scene3::createProjectile(int type) {
  
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

        _force_registry->add(p, _gravity_generator);
        _force_registry->add(p, _wind_generator);
        _force_registry->add(p, _whirlwind_generator);

        std::cout << "Proyectil tipo " << type << " disparado." << std::endl;
    }
}
