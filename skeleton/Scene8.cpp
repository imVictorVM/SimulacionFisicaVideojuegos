#include "Scene8.h"
#include "Render/Camera.h"
#include <iostream>
#include <sstream>
#include <algorithm>

extern PxPhysics* gPhysics;
extern PxScene* gScene;
extern std::string ui_text;

Scene8::Scene8() :
    _rb_registry(nullptr),
    _blue_spawner(nullptr), _red_spawner(nullptr),
    _buoyancy(nullptr), _spring_force(nullptr), _water_current(nullptr),
    _bonus_target(nullptr),
    _water_surface(nullptr), _water_transform(nullptr),
    _material(nullptr),
    _spawn_timer(0.0), _spawn_interval(2.0),
    _game_timer(0.0), _next_difficulty_increase(20.0),
    _shot_timer(0.25), _shot_cooldown(0.25),
    _score(0), _lives(3), _combo_counter(0),
    _is_game_over(false),
    _bonus_spawn_timer(0.0), _bonus_next_appearance(5.0),
    _camera_saved(false)
{
}

Scene8::~Scene8() {
    cleanup();
}

void Scene8::initialize() {
    std::cout << "Inicializando " << getDescription() << std::endl;

    // Configuración de la cámara: Solo se guarda la primera vez que se entra
    Camera* cam = GetCamera();
    if (cam && !_camera_saved) {
        _original_cam_eye = cam->getEye();
        _original_cam_dir = cam->getDir();
        _camera_saved = true;
        // Posición de juego
        cam->setEye({ 0.0f, 7.0f, 0.0f });
        cam->setDir({ 0.0f, -0.1f, -1.0f });
    }
    else if (cam) {
        // Restaurar posición de juego si venimos de un reset
        cam->setEye({ 0.0f, 7.0f, 0.0f });
        cam->setDir({ 0.0f, -0.1f, -1.0f });
    }

    _material = gPhysics->createMaterial(0.5f, 0.5f, 0.1f);

    // Iniciar la lógica del nivel
    resetLevel();
}

void Scene8::resetLevel() {
    // --- LIMPIEZA DE MEMORIA PREVIA ---
    // Eliminar entidades dinámicas
    for (auto s : _solids) delete s; _solids.clear();
    for (auto p : _projectiles) delete p; _projectiles.clear();
    if (_bonus_target) { delete _bonus_target; _bonus_target = nullptr; }

    // Eliminar generadores y fuerzas
    if (_rb_registry) delete _rb_registry;
    if (_blue_spawner) delete _blue_spawner;
    if (_red_spawner) delete _red_spawner;
    if (_buoyancy) delete _buoyancy;
    if (_spring_force) delete _spring_force;
    if (_water_current) delete _water_current;

    // Eliminar visuales del entorno
    if (_water_surface) { _water_surface->release(); _water_surface = nullptr; }
    if (_water_transform) { delete _water_transform; _water_transform = nullptr; }

    // --- REINICIALIZACIÓN DEL NIVEL ---
    _rb_registry = new RigidBodyForceRegistry();

    // Reset de variables de juego
    _is_game_over = false;
    _lives = 3; _score = 0; _combo_counter = 0;
    _spawn_timer = 0.0; _game_timer = 0.0;
    _spawn_interval = 2.0; _next_difficulty_increase = 20.0;
    _bonus_spawn_timer = 0.0; _bonus_next_appearance = 5.0 + (rand() % 10);

    // Configuración de generadores de enemigos
    // Cubos Azules
    _blue_spawner = new UniformSolidGenerator("Blue", gPhysics, gScene);
    _blue_spawner->setPosRange({ -6, 5, -50 }, { 6, 8, -50 });
    _blue_spawner->setVelRange({ 0, 0, 15 }, { 0, 0, 20 });
    _blue_spawner->setMaterial(_material);
    _blue_spawner->setGenerationColor({ 0.2f, 0.5f, 1.0f, 1.0f });
    _blue_spawner->setGenerationType(RigidTargetType::BLUE);
    _blue_spawner->setNumObjects(1);

    // Cubos Rojos
    _red_spawner = new UniformSolidGenerator("Red", gPhysics, gScene);
    _red_spawner->setPosRange({ -6, 5, -50 }, { 6, 8, -50 });
    _red_spawner->setVelRange({ 0, 0, 15 }, { 0, 0, 20 });
    _red_spawner->setMaterial(_material);
    _red_spawner->setGenerationColor({ 1.0f, 0.2f, 0.2f, 1.0f });
    _red_spawner->setGenerationType(RigidTargetType::RED);
    _red_spawner->setNumObjects(1);

    // Configuración del entorno (Agua)
    float waterLevel = -2.0f;
    _buoyancy = new SolidBuoyancyForceGenerator(waterLevel, 1000.0f); // Densidad agua
    _water_current = new SolidWindForceGenerator({ 0.0f, 0.0f, -10000.0f }); // Corriente hacia el fondo

    // Visual del agua (Extendido en profundidad)
    PxShape* water_shape = CreateShape(PxBoxGeometry(40.0f, 0.1f, 500.0f));
    _water_transform = new PxTransform({ 0.0f, waterLevel, -400.0f });
    _water_surface = new RenderItem(water_shape, _water_transform, { 0.1f, 0.3f, 0.8f, 0.5f });

    _bonus_anchor_pos = Vector3(0, 12, -15);

    updateUIText();
}

void Scene8::spawnBonus() {
    if (_bonus_target) return; // Evitar duplicados

    // Crear objeto físico (esfera) con gravedad activada
    _bonus_target = new RigidBody(gPhysics, gScene, { 0, 10, -15 }, PxSphereGeometry(1.5f), _material, 8.0f);
    _bonus_target->setColor({ 1.0f, 0.8f, 0.0f, 1.0f }); // Dorado
    _bonus_target->setType(RigidTargetType::BONUS);

    // Masa baja para facilitar el movimiento del muelle
    PxRigidBodyExt::updateMassAndInertia(*_bonus_target->getActor(), 0.1f);
    _bonus_target->getActor()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);

    // Crear y registrar el muelle
    _spring_force = new SolidAnchoredSpringForceGenerator(&_bonus_anchor_pos, 50.0f, 2.0f);
    _rb_registry->add(_bonus_target, _spring_force);
}

void Scene8::despawnBonus() {
    if (!_bonus_target) return;

    // Limpieza específica del bonus
    if (_rb_registry) _rb_registry->remove(_bonus_target, _spring_force);
    delete _bonus_target; _bonus_target = nullptr;
    delete _spring_force; _spring_force = nullptr;

    // Programar siguiente aparición
    _bonus_spawn_timer = 0.0;
    _bonus_next_appearance = 10.0 + (rand() % 10);
}

void Scene8::update(double t) {
    if (_is_game_over) return; // Pausar lógica en Game Over
    if (!_rb_registry) return;

    // --- GESTIÓN DE APARICIÓN DE BONUS ---
    if (_bonus_target == nullptr) {
        _bonus_spawn_timer += t;
        if (_bonus_spawn_timer > _bonus_next_appearance) spawnBonus();
    }
    else {
        _bonus_target->update((float)t);
        if (!_bonus_target->isAlive()) despawnBonus();
    }

    // --- DIFICULTAD PROGRESIVA ---
    _game_timer += t;
    if (_game_timer >= _next_difficulty_increase) {
        _next_difficulty_increase += 20.0;
        _spawn_interval = (std::max)(0.4, _spawn_interval * 0.9); // Aumentar frecuencia
    }

    // --- GENERACIÓN DE ENEMIGOS ---
    _spawn_timer += t;
    if (_spawn_timer >= _spawn_interval) {
        _spawn_timer = 0.0;
        std::list<RigidBody*> new_objs;

        // Aleatoriedad 50/50 Rojo/Azul
        if (rand() % 2 == 0) {
            _blue_spawner->setActive(true);
            new_objs = _blue_spawner->generateSolids();
        }
        else {
            _red_spawner->setActive(true);
            new_objs = _red_spawner->generateSolids();
        }

        for (auto rb : new_objs) {
            _solids.push_back(rb);
        }
    }

    // Actualizar cooldown de disparo
    if (_shot_timer < _shot_cooldown) _shot_timer += t;

    // Aplicar fuerzas físicas
    _rb_registry->updateForces(t);

    // --- BUCLE DE SÓLIDOS (Movimiento y Lógica) ---
    auto it = _solids.begin();
    while (it != _solids.end()) {
        RigidBody* rb = *it;
        rb->update(t);
        Vector3 pos = rb->getActor()->getGlobalPose().p;

        // Actualizar temporizador de impacto si fue golpeado
        if (rb->isHit()) rb->updateHitTimer((float)t);

        // Lógica de AGUA (Nivel Y < -2.5)
        if (pos.y < -2.5f) {
            _rb_registry->add(rb, _buoyancy);
            // Añadir damping alto para simular viscosidad
            rb->getActor()->setLinearDamping(3.0f);
            rb->getActor()->setAngularDamping(2.0f);
        }

        // Lógica de CORRIENTE (Retraso de 3 segundos tras impacto)
        if (rb->isHit() && rb->getHitTimer() > 3.0f) {
            _rb_registry->add(rb, _water_current);
        }

        // Lógica de FALLO (Enemigo rebasa al jugador)
        // Solo cuenta si no ha sido golpeado ya (!rb->isHit())
        if (pos.z > 5.0f && pos.y > 0.0f && rb->isAlive() && !rb->isHit()) {
            _lives--; _combo_counter = 0;
            updateUIText();

            // Activar físicas para que caiga
            rb->getActor()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false);

            // Marcar como "procesado" y cambiar visual
            rb->markAsHit();
            rb->setColor({ 0.5f, 0.5f, 0.5f, 1.0f }); // Gris
            rb->setLifetime(10.0f); // Tiempo extra para ver la caída
        }

        // Limpieza de objetos fuera de rango
        if (pos.z < -100.0f || pos.y < -50.0f || !rb->isAlive()) {
            _rb_registry->remove(rb, _buoyancy);
            _rb_registry->remove(rb, _water_current);
            delete rb;
            it = _solids.erase(it);
        }
        else {
            ++it;
        }
    }

    // --- BUCLE DE PROYECTILES Y COLISIONES ---
    auto it_proj = _projectiles.begin();
    while (it_proj != _projectiles.end()) {
        Particle* p = *it_proj;
        p->integrate(t);

        if (!p->isAlive()) {
            delete p;
            it_proj = _projectiles.erase(it_proj);
            continue;
        }

        bool hit = false;

        // Comprobar colisión con enemigos
        auto it_targ = _solids.begin();
        while (it_targ != _solids.end()) {
            RigidBody* t = *it_targ;

            // Colisión simple por distancia
            if (!t->isHit() && t->getActor()->getGlobalPose().p.y > -2.0f) {
                Vector3 t_pos = t->getActor()->getGlobalPose().p;
                if ((p->getPos() - t_pos).magnitude() < 2.5f) {

                    // Efectos de impacto
                    t->markAsHit();
                    t->setColor({ 0.5f, 0.5f, 0.5f, 1.0f }); // Visual: Gris
                    t->getActor()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false); // Físicas reales
                    t->setVelocity({ 0,0,0 }); // Frenar avance
                    t->addForce({ 0, -10000, -2000 }); // Empujón hacia agua/atrás
                    t->getActor()->addTorque({ 1000, 1000, 0 }); // Rotación

                    // Lógica de puntuación
                    if ((int)p->getTargetType() == (int)t->getType()) {
                        _score += 10 + (_combo_counter * 5);
                        _combo_counter++;
                    }
                    else {
                        _lives--; _combo_counter = 0;
                    }
                    updateUIText();

                    p->setLifetime(0); // Destruir proyectil
                    hit = true;
                    t->setLifetime(15.0f); // Tiempo para flotar
                    break;
                }
            }
            ++it_targ;
        }

        // Comprobar colisión con Bonus
        if (!hit && _bonus_target) {
            Vector3 b_pos = _bonus_target->getActor()->getGlobalPose().p;
            if ((p->getPos() - b_pos).magnitude() < 2.0f) {
                _score += 50;
                updateUIText();
                _bonus_target->addForce({ 0, 10000, 0 }); // Empujón fuerte
                _bonus_target->setLifetime(0.1f); // Desaparecer rápido
                p->setLifetime(0);
                hit = true;
            }
        }

        if (hit) it_proj++; else it_proj++;
    }

    // Comprobar Game Over
    if (_lives <= 0 && !_is_game_over) {
        _is_game_over = true;
        updateUIText();
    }
}

void Scene8::cleanup() {
    // Restaurar cámara si es necesario
    if (_camera_saved) {
        Camera* cam = GetCamera();
        if (cam) {
            cam->setEye(_original_cam_eye);
            cam->setDir(_original_cam_dir);
        }
    }

    // Limpieza de memoria
    if (_rb_registry) { delete _rb_registry; _rb_registry = nullptr; }
    for (auto s : _solids) delete s; _solids.clear();
    for (auto p : _projectiles) delete p; _projectiles.clear();

    if (_bonus_target) { delete _bonus_target; _bonus_target = nullptr; }
    if (_blue_spawner) { delete _blue_spawner; _blue_spawner = nullptr; }
    if (_red_spawner) { delete _red_spawner; _red_spawner = nullptr; }

    if (_buoyancy) { delete _buoyancy; _buoyancy = nullptr; }
    if (_spring_force) { delete _spring_force; _spring_force = nullptr; }
    if (_water_current) { delete _water_current; _water_current = nullptr; }

    if (_water_surface) { _water_surface->release(); _water_surface = nullptr; }
    if (_water_transform) { delete _water_transform; _water_transform = nullptr; }

    if (_material) { _material->release(); _material = nullptr; }
}

void Scene8::handleKeyPress(unsigned char key) {
    // Modo Game Over: Solo permite reiniciar
    if (_is_game_over) {
        if (toupper(key) == 'R') {
            resetLevel();
        }
        return;
    }

    // Controles de juego
    switch (toupper(key)) {
    case 'B': if (_shot_timer >= _shot_cooldown) { createProjectile(1); _shot_timer = 0.0; } break;
    case 'C': if (_shot_timer >= _shot_cooldown) { createProjectile(2); _shot_timer = 0.0; } break;
    }
}

void Scene8::updateUIText() {
    std::stringstream ss;
    if (_is_game_over) {
        ss << "=== GAME OVER ===" << std::endl;
        ss << "Puntuacion Final: " << _score << std::endl;
        ss << "Pulsa 'R' para reiniciar";
    }
    else {
        ss << "Vidas: " << _lives << std::endl << "Puntuacion: " << _score;
        if (_combo_counter > 1) ss << "\nCombo: x" << _combo_counter;
    }
    ui_text = ss.str();
}

void Scene8::createProjectile(int type) {
    Camera* cam = GetCamera();
    if (!cam) return;
    Vector3 pos = cam->getEye();
    Vector3 dir = cam->getDir();
    Projectile* p = nullptr;
    if (type == 1) {
        p = new LaserPistol(pos, dir * 250.0f);
        p->setColor({ 0.2f, 0.5f, 1.0f, 1.0f });
        p->setTargetType(TargetType::BLUE);
    }
    else {
        p = new LaserPistol(pos, dir * 250.0f);
        p->setColor({ 1.0f, 0.2f, 0.2f, 1.0f });
        p->setTargetType(TargetType::RED);
    }
    p->setupVisual();
    _projectiles.push_back(p);
}