#pragma once
#include "Scene.h"
#include "core.hpp"
#include <vector>
#include <string>

class Projectile;
class ParticleForceRegistry;
class GravityForceGenerator;

class Scene1 : public Scene {
public:
    Scene1();
    virtual ~Scene1();

    void initialize() override;
    void update(double t) override;
    void cleanup() override;
    void handleKeyPress(unsigned char key) override;
    std::string getDescription() const override { return "Escena 1: Proyectiles con Fuerzas"; }

private:
    void createProjectile(int type);


    std::vector<Projectile*> _projectiles;

    ParticleForceRegistry* _force_registry;
    GravityForceGenerator* _gravity_generator;
};