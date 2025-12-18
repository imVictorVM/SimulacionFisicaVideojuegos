#pragma once
#include "Scene.h"
#include "RigidBody.h"
#include "RigidBodyForceRegistry.h"
#include "UniformSolidGenerator.h"
#include "ExplosionSolidForceGenerator.h"
#include "SolidSpringForceGenerator.h"
#include "SolidBuoyancyForceGenerator.h"
#include "WindForceGenerator.h"
#include "LaserPistol.h"
#include <list>
#include <vector>

// Necesitamos un generador de viento para SÓLIDOS para la corriente del agua
class SolidWindForceGenerator : public SolidForceGenerator {
public:
    SolidWindForceGenerator(const Vector3& wind) : _wind(wind) {}
    virtual void updateForce(RigidBody* solid, float t) override {
        // Viento simple sin área
        solid->addForce(_wind);
    }
private:
    Vector3 _wind;
};


class Scene8 : public Scene {
public:
    Scene8();
    virtual ~Scene8();

    void initialize() override;
    void update(double t) override;
    void cleanup() override;
    void handleKeyPress(unsigned char key) override;
    std::string getDescription() const override { return "Proyecto Final: PhysX Saber"; }

private:
    void createProjectile(int type);
    void updateUIText();

    RigidBodyForceRegistry* _rb_registry;
    std::list<RigidBody*> _solids;

    UniformSolidGenerator* _blue_spawner;
    UniformSolidGenerator* _red_spawner;

    ExplosionSolidForceGenerator* _explosion;
    SolidBuoyancyForceGenerator* _buoyancy;
    SolidAnchoredSpringForceGenerator* _spring_force;
    SolidWindForceGenerator* _water_current;

    RigidBody* _bonus_target;
    Vector3 _bonus_anchor_pos;
    RenderItem* _water_surface;

    std::list<Particle*> _projectiles;

    double _spawn_timer;
    double _spawn_interval;
    double _game_timer;
    double _next_difficulty_increase;
    double _bomb_timer;
    double _bomb_cooldown;
    double _shot_timer;
    double _shot_cooldown;

    int _score;
    int _lives;
    int _combo_counter;

    PxMaterial* _material;

    physx::PxVec3 _original_cam_eye;
    physx::PxVec3 _original_cam_dir;
};