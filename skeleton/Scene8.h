#pragma once
#include "Scene.h"
#include "RigidBody.h"
#include "RigidBodyForceRegistry.h"
#include "UniformSolidGenerator.h"
#include "SolidSpringForceGenerator.h"
#include "SolidBuoyancyForceGenerator.h"
#include "SolidWindForceGenerator.h"
#include "LaserPistol.h"
#include <list>
#include <vector>

class Scene8 : public Scene {
public:
    Scene8();
    virtual ~Scene8();

    // Métodos principales del ciclo de vida de la escena
    void initialize() override;
    void update(double t) override;
    void cleanup() override;
    void handleKeyPress(unsigned char key) override;

    std::string getDescription() const override { return "Proyecto Final: PhysX Saber"; }

private:
    // Métodos auxiliares
    void createProjectile(int type);
    void updateUIText();

    // Gestión del objeto de bonus
    void spawnBonus();
    void despawnBonus();

    // Reinicia el nivel sin perder la configuración de la cámara
    void resetLevel();

    // --- FÍSICAS ---
    RigidBodyForceRegistry* _rb_registry;

    // Generadores de enemigos (cubos)
    UniformSolidGenerator* _blue_spawner;
    UniformSolidGenerator* _red_spawner;

    // Generadores de fuerza del entorno
    SolidBuoyancyForceGenerator* _buoyancy;      // Flotación del agua
    SolidWindForceGenerator* _water_current;     // Corriente del río
    SolidAnchoredSpringForceGenerator* _spring_force; // Muelle del bonus

    // --- ENTIDADES ---
    std::list<RigidBody*> _solids;        // Lista de enemigos activos
    std::list<Particle*> _projectiles;    // Lista de disparos activos

    RigidBody* _bonus_target;             // Objeto de bonificación
    Vector3 _bonus_anchor_pos;            // Punto de anclaje del muelle

    // Elementos visuales
    RenderItem* _water_surface;
    physx::PxTransform* _water_transform;

    // --- LÓGICA DE JUEGO ---
    // Control de aparición de enemigos
    double _spawn_timer;
    double _spawn_interval;
    double _game_timer;
    double _next_difficulty_increase;

    // Control de disparo (cooldown)
    double _shot_timer;
    double _shot_cooldown;

    // Control de aparición del bonus
    double _bonus_spawn_timer;
    double _bonus_next_appearance;

    // Estado del jugador
    int _score;
    int _lives;
    int _combo_counter;
    bool _is_game_over;

    // Material físico compartido
    PxMaterial* _material;

    // Respaldo de la cámara para restaurarla al salir
    physx::PxVec3 _original_cam_eye;
    physx::PxVec3 _original_cam_dir;
    bool _camera_saved = false;
};