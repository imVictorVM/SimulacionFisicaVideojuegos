#include "Scene1.h"
#include "core.hpp"
#include "RenderUtils.hpp"
#include <iostream>

void Scene1::initialize() {
    std::cout << "Inicializando " << getDescription() << std::endl;


}

void Scene1::cleanup()
{

    for (auto p : projectiles) {
        p->cleanup();
    }
    projectiles.clear();
}

void Scene1::handleKeyPress(unsigned char key) {
    switch (toupper(key)) {
    case 'B': // Crear bala
        createProjectile(1);
        std::cout << "Bala creada" << std::endl;
        break;
    case 'C': // Crear bola de cañón
        createProjectile(2);
        std::cout << "Canon creada" << std::endl;
        break;
    case 'T': // Crear bola de tanque
        createProjectile(3);
        std::cout << "Tanque creada" << std::endl;
        break;
    case 'L': // Crear pistola laser
        createProjectile(4);
        std::cout << "Laser creada" << std::endl;
        break;
    case ' ': // Limpiar proyectiles
        for (auto p : projectiles) {
            delete p;
        }
        projectiles.clear();
        break;
    }
}

void Scene1::createProjectile(int type) {
    Camera* cam = GetCamera();
    Vector3 startPos = cam->getEye();
    Vector3 direction = cam->getDir();

    Projectile* projectile = nullptr;

    switch (type) {
    case 1: // Bullet
        projectile = new Bullet(startPos, direction * 80.0f);
        break;
    case 2: // CannonBall
        projectile = new CannonBall(startPos, direction * 40.0f);
        break;
    case 3: // TankShell
        projectile = new TankBall(startPos, direction * 150.0f);
        break;
    case 4: // LaserPistol
        projectile = new LaserPistol(startPos, direction * 250.0f);
        break;
    }

    if (projectile != nullptr) {
        projectile->setupVisual();
        projectiles.push_back(projectile);
    }
}