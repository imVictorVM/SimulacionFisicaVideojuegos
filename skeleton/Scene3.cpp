#include "Scene3.h"
#include "Render/Camera.h"
#include "ParticleForceRegistry.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"
#include "WhirlwindForceGenerator.h"
#include <iostream>
// NOTA: No necesitamos incluir "Bullet.h", etc. aquí
// porque ya están incluidos en "Scene3.h"

Scene3::Scene3() :
    _force_registry(nullptr),
    _gravity_generator(nullptr),
    _wind_generator(nullptr),
    _whirlwind_generator(nullptr)
{
}

Scene3::~Scene3() {
    cleanup();
}

void Scene3::initialize() {
    std::cout << "Inicializando " << getDescription() << std::endl;

    // --- ARREGLO DE CÁMARA ---
    Camera* cam = GetCamera();
    if (cam) {
        _original_cam_eye = cam->getEye();
        _original_cam_dir = cam->getDir();
        cam->setEye({ 0.0f, 15.0f, 40.0f });
        cam->setDir({ 0.0f, -0.3f, -1.0f });
    }

    // --- CREACIÓN DE FUERZAS ---
    _force_registry = new ParticleForceRegistry();

    // 1. Gravedad (afecta a todo)
    _gravity_generator = new GravityForceGenerator({ 0.0f, -9.8f, 0.0f });

    // 2. Zona de Viento (a la izquierda)
    _wind_generator = new WindForceGenerator({ 50.0f, 0.0f, 0.0f });
    _wind_generator->setWindArea({ -25.0f, 0.0f, 0.0f }, { -5.0f, 20.0f, 30.0f });

    // 3. Zona de Torbellino (a la derecha)
    _whirlwind_generator = new WhirlwindForceGenerator(150.0f, { 15.0f, 0.0f, 15.0f }, { 0.0f, 1.0f, 0.0f });
    _whirlwind_generator->setWindArea({ 5.0f, 0.0f, 0.0f }, { 25.0f, 20.0f, 30.0f });
}

void Scene3::update(double t) {
    // 1. Aplicar todas las fuerzas
    _force_registry->updateForces(t);

    // 2. Integrar y limpiar partículas/proyectiles
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
    Camera* cam = GetCamera();
    if (cam) {
        cam->setEye(_original_cam_eye);
        cam->setDir(_original_cam_dir);
    }

    // --- Limpiar partículas ---
    for (auto p : _projectiles) {
        _force_registry->remove(p, _gravity_generator);
        _force_registry->remove(p, _wind_generator);
        _force_registry->remove(p, _whirlwind_generator);
        delete p;
    }
    _projectiles.clear();

    // --- Limpiar registro y generadores ---
    delete _force_registry;
    _force_registry = nullptr;
    delete _gravity_generator;
    _gravity_generator = nullptr;
    delete _wind_generator;
    _wind_generator = nullptr;
    delete _whirlwind_generator;
    _whirlwind_generator = nullptr;
}

// --- handleKeyPress TOTALMENTE ACTUALIZADO ---
void Scene3::handleKeyPress(unsigned char key) {
    switch (toupper(key)) {
    case 'B': createProjectile(1); break; // Bullet
    case 'C': createProjectile(2); break; // CannonBall
    case 'T': createProjectile(3); break; // TankBall
    case 'L': createProjectile(4); break; // LaserPistol
    case ' ': // Tecla de espacio para limpiar la escena
        for (auto p : _projectiles) {
            _force_registry->remove(p, _gravity_generator);
            _force_registry->remove(p, _wind_generator);
            _force_registry->remove(p, _whirlwind_generator);
            delete p;
        }
        _projectiles.clear();
        std::cout << "Escena 3 limpiada." << std::endl;
        break;
    }
}

// --- createProjectile AÑADIDO (copiado de Scene1) ---
void Scene3::createProjectile(int type) {
    Camera* cam = GetCamera();
    Vector3 startPos = cam->getEye();
    Vector3 direction = cam->getDir();
    Projectile* p = nullptr; // Usamos Projectile* ya que es la base común

    switch (type) {
    case 1: p = new Bullet(startPos, direction * 80.0f); break;
    case 2: p = new CannonBall(startPos, direction * 40.0f); break;
    case 3: p = new TankBall(startPos, direction * 150.0f); break;
    case 4: p = new LaserPistol(startPos, direction * 250.0f); break;
    }

    if (p) {
        p->setupVisual();
        _projectiles.push_back(p); // Lo añadimos a nuestro vector de Particle*

        // --- ¡LA PARTE MÁS IMPORTANTE! ---
        // Registramos el nuevo proyectil en TODOS los generadores de fuerza
        _force_registry->add(p, _gravity_generator);
        _force_registry->add(p, _wind_generator);
        _force_registry->add(p, _whirlwind_generator);

        std::cout << "Proyectil tipo " << type << " disparado." << std::endl;
    }
}