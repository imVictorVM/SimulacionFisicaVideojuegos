#pragma once
#include "Scene.h"
#include "core.hpp"
#include <vector>

// --- INCLUDES AÑADIDOS ---
#include "Bullet.h"
#include "CannonBall.h"
#include "TankBall.h"
#include "LaserPistol.h"

// Forward declarations
class Projectile;
class ParticleForceRegistry;
class GravityForceGenerator;
class WindForceGenerator;
class WhirlwindForceGenerator;

class Scene3 : public Scene {
public:
    Scene3();
    virtual ~Scene3();

    void initialize() override;
    void update(double t) override;
    void cleanup() override;
    void handleKeyPress(unsigned char key) override;
    std::string getDescription() const override { return "Escena 3: Viento y Torbellino"; }

private:
    void createProjectile(int type);

    std::vector<Projectile*> _projectiles;

    ParticleForceRegistry* _force_registry;
    GravityForceGenerator* _gravity_generator;
    WindForceGenerator* _wind_generator;
    WhirlwindForceGenerator* _whirlwind_generator;

    physx::PxVec3 _original_cam_eye;
    physx::PxVec3 _original_cam_dir;
};