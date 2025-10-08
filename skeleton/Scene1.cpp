#include "Scene1.h"
#include "core.hpp"
#include "RenderUtils.hpp"
#include <iostream>

void Scene1::initialize() {
    std::cout << "Inicializando " << getDescription() << std::endl;


}

void Scene1::handleKeyPress(unsigned char key) {
    switch (toupper(key)) {
    case 'B': // Crear bala
        createProjectile(1);
        std::cout << "Bala creada" << std::endl;
        break;
    case 'C': // Crear bola de cañón
        createProjectile(2);
        std::cout << "Bola de cañón creada" << std::endl;
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
    case 1: // Bala
        projectile = new Bullet(startPos, direction * 80.0f);
        break;
    case 2: // Bola de cañón
        projectile = new CannonBall(startPos, direction * 40.0f);
        break;
    }

    if (projectile) {
        projectile->setupVisual();
        projectiles.push_back(projectile);
    }
}