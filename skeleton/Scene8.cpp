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
    _blue_spawner(nullptr),
    _red_spawner(nullptr),
    _explosion(nullptr),
    _buoyancy(nullptr),
    _spring_force(nullptr),
    _water_current(nullptr),
    _bonus_target(nullptr),
    _water_surface(nullptr),
    _material(nullptr),
    _spawn_timer(0.0), _spawn_interval(2.0),
    _game_timer(0.0), _next_difficulty_increase(20.0),
    _bomb_timer(15.0), _bomb_cooldown(15.0),
    _shot_timer(0.25), _shot_cooldown(0.25),
    _score(0), _lives(3), _combo_counter(0)
{
}

Scene8::~Scene8() { cleanup(); }

void Scene8::initialize() {
    std::cout << "Inicializando " << getDescription() << std::endl;
    updateUIText();

    Camera* cam = GetCamera();
    if (cam) {
        _original_cam_eye = cam->getEye();
        _original_cam_dir = cam->getDir();
        cam->setEye({ 0.0f, 5.0f, 0.0f });
        cam->setDir({ 0.0f, 0.0f, -1.0f });
    }

    _rb_registry = new RigidBodyForceRegistry();
    _material = gPhysics->createMaterial(0.5f, 0.5f, 0.1f);

    // GENERADORES (Vienen de Z=-50 hacia Z=0)
    _blue_spawner = new UniformSolidGenerator("Blue", gPhysics, gScene);
    _blue_spawner->setPosRange({ -8, 2, -50 }, { 8, 8, -50 });
    _blue_spawner->setVelRange({ 0, 0, 15 }, { 0, 0, 25 });
    _blue_spawner->setMaterial(_material);
    _blue_spawner->setGenerationColor({ 0.2f, 0.5f, 1.0f, 1.0f });
    _blue_spawner->setNumObjects(1);

    _red_spawner = new UniformSolidGenerator("Red", gPhysics, gScene);
    _red_spawner->setPosRange({ -8, 2, -50 }, { 8, 8, -50 });
    _red_spawner->setVelRange({ 0, 0, 15 }, { 0, 0, 25 });
    _red_spawner->setMaterial(_material);
    _red_spawner->setGenerationColor({ 1.0f, 0.2f, 0.2f, 1.0f });
    _red_spawner->setNumObjects(1);

    // FUERZAS
    _explosion = new ExplosionSolidForceGenerator(5000000.0, 0.5, 60.0, { 0, 5, -20 });

    // AGUA Y CORRIENTE
    // El agua está "debajo" del camino principal
    _buoyancy = new SolidBuoyancyForceGenerator(-5.0f, 1000.0f);
    // Corriente fuerte hacia atrás, alejándose del jugador
    _water_current = new SolidWindForceGenerator({ 0.0f, 0.0f, -200.0f });

    // Visual del agua
    PxShape* water_shape = CreateShape(PxBoxGeometry(50.0f, 0.1f, 100.0f));
    PxTransform water_t({ 0.0f, -5.0f, -30.0f });
    _water_surface = new RenderItem(water_shape, &water_t, { 0.2f, 0.5f, 1.0f, 0.4f });

    // BONUS (Campana)
    _bonus_anchor_pos = Vector3(0, 12, -15);
    _bonus_target = new RigidBody(gPhysics, gScene, { 0, 10, -15 }, PxSphereGeometry(1.5f), _material);
    _bonus_target->setColor({ 1.0f, 0.8f, 0.0f, 1.0f });
    _bonus_target->setType(RigidTargetType::BONUS);
    PxRigidBodyExt::updateMassAndInertia(*_bonus_target->getActor(), 0.1f);

    _spring_force = new SolidAnchoredSpringForceGenerator(&_bonus_anchor_pos, 50.0f, 2.0f);
    _spring_force->setupVisual(CreateShape(PxBoxGeometry(0.2f, 0.2f, 0.2f)));

    _rb_registry->add(_bonus_target, _spring_force);
}

void Scene8::update(double t) {
    if (!_rb_registry) return;

    _game_timer += t;
    if (_game_timer >= _next_difficulty_increase) {
        _next_difficulty_increase += 20.0;
        _spawn_interval = (std::max)(0.4, _spawn_interval * 0.9);
        std::cout << "¡SPEED UP!" << std::endl;
    }

    _spawn_timer += t;
    if (_spawn_timer >= _spawn_interval) {
        _spawn_timer = 0.0;
        std::list<RigidBody*> new_objs;
        if (rand() % 2 == 0) {
            _blue_spawner->setActive(true);
            new_objs = _blue_spawner->generateSolids();
            for (auto rb : new_objs) rb->setType(RigidTargetType::BLUE);
        }
        else {
            _red_spawner->setActive(true);
            new_objs = _red_spawner->generateSolids();
            for (auto rb : new_objs) rb->setType(RigidTargetType::RED);
        }
        for (auto rb : new_objs) {
            _solids.push_back(rb);
            _rb_registry->add(rb, _explosion);
        }
    }

    if (_bomb_timer < _bomb_cooldown) _bomb_timer += t;
    if (_shot_timer < _shot_cooldown) _shot_timer += t;
    if (_explosion) _explosion->update(t);

    _rb_registry->updateForces(t);

    // --- LÓGICA DE OBJETOS ---
    auto it = _solids.begin();
    while (it != _solids.end()) {
        RigidBody* rb = *it;
        rb->update(t);
        Vector3 pos = rb->getActor()->getGlobalPose().p;

        // Si están muy bajos (cayeron al río), aplicar flotación y corriente
        if (pos.y < -3.0f) {
            _rb_registry->add(rb, _buoyancy);
            _rb_registry->add(rb, _water_current);
        }

        // Si pasan al jugador (Z > 2), FALLO
        if (pos.z > 2.0f && rb->isAlive()) { // Vivo y pasó
            _lives--; _combo_counter = 0;
            updateUIText();
            std::cout << "FALLO (pasado)" << std::endl;
            rb->setLifetime(0); // "Muerto" para el juego, pero sigue en física

            // Empujón para que caigan al agua detrás
        }

        // Limpieza real
        if (pos.z < -100.0f || pos.y < -20.0f) {
            _rb_registry->remove(rb, _explosion);
            _rb_registry->remove(rb, _buoyancy);
            _rb_registry->remove(rb, _water_current);
            delete rb;
            it = _solids.erase(it);
        }
        else {
            ++it;
        }
    }

    // --- COLISIONES ---
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
        auto it_targ = _solids.begin();
        while (it_targ != _solids.end()) {
            RigidBody* t = *it_targ;

            // Solo comprobar si el objetivo está "vivo"
            if (t->isAlive()) {
                Vector3 t_pos = t->getActor()->getGlobalPose().p;

                if ((p->getPos() - t_pos).magnitude() < 2.0f) {
                    // COMPARACIÓN DE TIPOS
                    auto pType = p->getTargetType();
                    auto tType = t->getType();
                    if ((pType == TargetType::BLUE && tType == RigidTargetType::BLUE) || (pType == TargetType::RED && tType == RigidTargetType::RED)) {
                        _score += 10 + (_combo_counter * 5);
                        _combo_counter++;
                        std::cout << "HIT!" << std::endl;

                        // EFECTO DE GOLPE:
                        // 1. Quitarle velocidad hacia adelante
                        t->setVelocity({ 0,0,0 });
                        // 2. Empujarlo hacia abajo/atrás para que caiga al agua
                        t->addForce({ 0, -2000, -1000 });
                        // 3. Marcar como "muerto" para no volver a golpearlo ni contar fallo
                        t->setLifetime(0);

                    }
                    else {
                        _lives--; _combo_counter = 0;
                        std::cout << "WRONG COLOR" << std::endl;
                    }
                    updateUIText();
                    p->setLifetime(0);
                    hit = true;
                    break;
                }
            }
            ++it_targ;
        }


        if (hit) it_proj++;
        else it_proj++;
    }

    if (_lives <= 0) {
        std::cout << "GAME OVER" << std::endl;
        cleanup();
        initialize();
    }
}

void Scene8::cleanup() {


    for (auto s : _solids) delete s; _solids.clear();
    for (auto p : _projectiles) delete p; _projectiles.clear();
    if (_bonus_target) { delete _bonus_target; _bonus_target = nullptr; }
    if (_blue_spawner) delete _blue_spawner;
    if (_red_spawner) delete _red_spawner;
    if (_rb_registry) delete _rb_registry;
    if (_explosion) delete _explosion;
    if (_buoyancy) delete _buoyancy;
    if (_spring_force) delete _spring_force;
    if (_water_current) delete _water_current;
    if (_water_surface) _water_surface->release();
    if (_material) _material->release();
}


void Scene8::handleKeyPress(unsigned char key) {
    switch (toupper(key)) {
    case 'B': // Disparo AZUL
        if (_shot_timer >= _shot_cooldown) {
            createProjectile(1);
            _shot_timer = 0.0;
        }
        break;
    case 'C': // Disparo ROJO
        if (_shot_timer >= _shot_cooldown) {
            createProjectile(2);
            _shot_timer = 0.0;
        }
        break;
    case 'E': // Activar BOMBA
        if (_explosion && _bomb_timer >= _bomb_cooldown) {
            _explosion->trigger();
            _bomb_timer = 0.0;
        }
        break;
    }
}

void Scene8::updateUIText() {
    std::stringstream ss;
    ss << "Vidas: " << _lives << std::endl << "Puntuacion: " << _score;
    if (_combo_counter > 1) ss << "\nCombo: x" << _combo_counter;
    ui_text = ss.str();
}

void Scene8::createProjectile(int type) {
    Camera* cam = GetCamera();
    if (!cam) return;
    Vector3 pos = cam->getEye();
    Vector3 dir = cam->getDir();
    Projectile* p = nullptr;
    if (type == 1) { // AZUL
        p = new LaserPistol(pos, dir * 250.0f);
        p->setColor({ 0.2f, 0.5f, 1.0f, 1.0f });
        p->setTargetType(TargetType::BLUE);
    }
    else { // ROJO
        p = new LaserPistol(pos, dir * 250.0f);
        p->setColor({ 1.0f, 0.2f, 0.2f, 1.0f });
        p->setTargetType(TargetType::RED);
    }
    p->setupVisual();
    _projectiles.push_back(p);
}