#pragma once
#include "Scene.h"
#include "ParticleSystem.h"
#include "ParticleForceRegistry.h"
#include "GaussianGenerator.h"
#include "UniformGenerator.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"
#include "ExplosionForceGenerator.h"
#include "Projectile.h"
#include <vector>
#include <list>
#include <string>

class Scene8 : public Scene {
public:
    Scene8();
    virtual ~Scene8();

    void initialize() override;
    void update(double t) override;
    void cleanup() override;
    void handleKeyPress(unsigned char key) override;
    std::string getDescription() const override { return "Proyecto intermedio"; }

private:

    void updateUIText();

    // --- Lógica del Jugador (P1) ---
    void createProjectile(int type);

    std::list<Particle*> _projectiles;
    GravityForceGenerator* _projectile_gravity;

    // --- Lógica de Objetivos (P2) ---
    ParticleSystem* _target_spawner;
    UniformGenerator* _red_spawner;
    UniformGenerator* _blue_spawner;
    double _spawn_timer;
    double _spawn_interval;

    // --- Lógica de Físicas (P3) ---
    ParticleForceRegistry* _force_registry;
    GravityForceGenerator* _target_mover; // Fuerza que atrae los objetivos
    WindForceGenerator* _side_wind;       // Fuerza que desvía los objetivos
    ExplosionForceGenerator* _bomb;        // Fuerza que destruye objetivos
    WindForceGenerator* _target_drag; //Fuerza que hace de drag

    double _wind_timer;
    double _wind_interval;

    // --- Lógica de Juego ---
    int _score;
    int _lives;

    double _game_timer;
    double _next_difficulty_increase;

    double _bomb_timer;
    double _bomb_cooldown;

    double _shot_timer;
    double _shot_cooldown;

    int _combo_counter;

    // --- Cámara ---
    physx::PxVec3 _original_cam_eye;
    physx::PxVec3 _original_cam_dir;
};