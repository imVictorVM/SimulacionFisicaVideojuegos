#include "Scene5.h"
#include "Render/Camera.h"
#include "Bullet.h" 
#include <iostream>
#include "LaserPistol.h"
#include "BoxParticle.h"

Scene5::Scene5() :
    _force_registry(nullptr),
    _projectile_gravity(nullptr),
    _target_spawner(nullptr),
    _red_spawner(nullptr),
    _blue_spawner(nullptr),
    _target_mover(nullptr),
    _side_wind(nullptr),
    _bomb(nullptr),
    _target_drag(nullptr),
    _spawn_timer(0.0),
    _spawn_interval(2.0),
    _wind_timer(0.0),
    _wind_interval(5.0),
    _score(0),
    _lives(3),
    _game_timer(0.0),
    _next_difficulty_increase(20.0),
    _bomb_timer(15.0),
    _bomb_cooldown(15.0),
    _shot_timer(0.25),
    _shot_cooldown(0.25)
{
}

Scene5::~Scene5() {
    cleanup();
}

void Scene5::initialize() {
    std::cout << "Inicializando " << getDescription() << std::endl;
    std::cout << "Puntuacion: " << _score << " | Vidas: " << _lives << std::endl;

    //0 Posicionar Cámara Fija
    Camera* cam = GetCamera();
    if (cam) {
        _original_cam_eye = cam->getEye();
        _original_cam_dir = cam->getDir();
        cam->setEye({ 0.0f, 5.0f, 0.0f });
        cam->setDir({ 0.0f, 0.0f, -1.0f }); 
    }

    //1 Crear Sistemas
    _force_registry = new ParticleForceRegistry();
    _target_spawner = new ParticleSystem();

    //2 Crear Fuerzas
    _projectile_gravity = new GravityForceGenerator({ 0.0f, -9.8f, 0.0f });
    _target_mover = new GravityForceGenerator({ 0.0f, 0.0f, 20.0f }); // Atrae objetivos hacia Z+

    _target_drag = new WindForceGenerator(Vector3(0.0f), 2.0f); // k1 = 2.0
    _target_drag->setWindArea({ -30.0f, -10.0f, -40.0f }, { 30.0f, 20.0f, 30.0f });

    _side_wind = new WindForceGenerator({ 15.0f, 0.0f, 0.0f }); // Viento lateral
    _bomb = new ExplosionForceGenerator(1000000.0f, 0.5f, 50.0f, { 0, 5, -20 }); // Bomba

    //Configurar área de viento
    _side_wind->setWindArea({ -30.0f, -10.0f, -40.0f }, { 30.0f, 20.0f, 10.0f });


    //3 Crear Plantillas de Objetivos
    //Plantilla OBJETIVO AZUL
    Particle* blue_target_model = new BoxParticle(
        { 0,0,0 }, { 0,0,0 },
        1.0, 1.0, 10.0
    );
    blue_target_model->setColor({ 0.2f, 0.5f, 1.0f, 1.0f });
    blue_target_model->setRadius(1.5f);
    blue_target_model->setupVisual();

    //Plantilla OBJETIVO ROJO
    Particle* red_target_model = new BoxParticle(
        { 0,0,0 }, { 0,0,0 },
        1.0, 1.0, 10.0
    );
    red_target_model->setColor({ 1.0f, 0.2f, 0.2f, 1.0f });
    red_target_model->setRadius(1.5f);
    red_target_model->setupVisual();

    blue_target_model->setTargetType(TargetType::BLUE);
    red_target_model->setTargetType(TargetType::RED);

    //4 Crear Generadores (Spawners)
    _blue_spawner = new UniformGenerator("Blue_Spawner");
    _blue_spawner->setParticleModel(blue_target_model);

    _blue_spawner->setPosRange({ -10.0f, 2.0f, -50.0f }, { 10.0f, 8.0f, -50.0f });
    _blue_spawner->setVelRange({ 0,0,0 }, { 0,0,0 });
    _blue_spawner->setNumParticles(1);
    _blue_spawner->setActive(false);

    _red_spawner = new UniformGenerator("Red_Spawner");
    _red_spawner->setParticleModel(red_target_model);
    _red_spawner->setPosRange({ -10.0f, 2.0f, -50.0f }, { 10.0f, 8.0f, -50.0f });
    _red_spawner->setVelRange({ 0,0,0 }, { 0,0,0 });
    _red_spawner->setNumParticles(1);
    _red_spawner->setActive(false);

    //5 Añadir generadores al sistema
    _target_spawner->addGenerator(_blue_spawner);
    _target_spawner->addGenerator(_red_spawner);
}

void Scene5::update(double t) {
    if (!_force_registry || !_target_spawner) return;


    //0 Lógica de Dificultad incremental
    _game_timer += t;
    if (_game_timer >= _next_difficulty_increase) {
        //1 Establecer el siguiente nivel de dificultad
        _next_difficulty_increase += 20.0; // Siguiente aumento en 20s

        //2 Aumentar la velocidad de aparición
        _spawn_interval = (std::max)(0.5, _spawn_interval * 0.9); // 10% más rápido

        // 3. Aumentar la velocidad de los objetivos
        if (_target_mover) {
            Vector3 current_gravity = _target_mover->getGravity();
            _target_mover->setGravity(current_gravity * 1.15f); // 15% más rápido
        }

        //4 Reducir el tamaño de los objetivos
        if (_red_spawner) {
            // Reduce el tamaño un 5%, con un mínimo de 0.5
            _red_spawner->scaleModelParticleRadius(0.95f, 0.5f);
        }
        if (_blue_spawner) {
            _blue_spawner->scaleModelParticleRadius(0.95f, 0.5f);
        }

        std::cout << "\n--- ¡DIFICULTAD AUMENTADA! ---" << std::endl;
        std::cout << "Nuevo intervalo de spawn: " << _spawn_interval << "s" << std::endl;
        std::cout << "Nueva fuerza de objetivo: " << _target_mover->getGravity().z << std::endl;
    }


    //1 Lógica de Spawning
    _spawn_timer += t;
    std::list<Particle*> new_targets;

    if (_spawn_timer >= _spawn_interval) {
        _spawn_timer = 0.0;
        if (rand() % 2 == 0) {
            _blue_spawner->setActive(true);
            new_targets = _target_spawner->update(0);
            _blue_spawner->setActive(false);
        }
        else {
            _red_spawner->setActive(true);
            new_targets = _target_spawner->update(0);
            _red_spawner->setActive(false);
        }
    }

    _wind_timer += t;
    if (_wind_timer >= _wind_interval) {
        _wind_timer = 0.0;
        if (_side_wind) {
            _side_wind->toggleActive();
            // Imprime en la consola el estado del viento
            std::cout << "Viento lateral: " << (_side_wind->isActive() ? "ON" : "OFF") << std::endl;
        }
    }

    //Timers de cooldown de disparo y explosión
    if (_bomb_timer < _bomb_cooldown) {
        _bomb_timer += t;
    }

    if (_shot_timer < _shot_cooldown) {
        _shot_timer += t;
    }

    //2 REGISTRAR FUERZAS NUEVAS
    for (Particle* target : new_targets) {
        _force_registry->add(target, _target_mover);
        _force_registry->add(target, _side_wind);
        _force_registry->add(target, _bomb);
        _force_registry->add(target, _target_drag);
    }

    //3 ACTUALIZAR FUERZAS
    _force_registry->updateForces(t);

    //4 INTEGRAR Y LIMPIAR PROYECTILES
    auto it_proj = _projectiles.begin();
    while (it_proj != _projectiles.end()) {
        Particle* p = *it_proj;
        p->integrate(t);

        if (!p->isAlive()) {
            _force_registry->remove(p, _projectile_gravity);
            delete p;
            it_proj = _projectiles.erase(it_proj);
        }
        else {
            ++it_proj;
        }
    }

    //5 INTEGRAR OBJETIVOS
    _target_spawner->update(t);

    //6 LÓGICA DE JUEGO (Colisiones)
    auto& targets = _target_spawner->getParticles();

    for (it_proj = _projectiles.begin(); it_proj != _projectiles.end(); ++it_proj) {
        Particle* p = *it_proj;

        auto it_targ = targets.begin();
        while (it_targ != targets.end()) {
            Particle* t = *it_targ;

            //Solo comprobar colisiones con partículas vivas
            if (p->isAlive() && t->isAlive()) {
                float distance = (p->getPos() - t->getPos()).magnitude();

                float radius_sum = p->getRadius() + t->getRadius();

                if (distance < radius_sum) {
                    if (p->getTargetType() == t->getTargetType()) {
                        _score += 10;
                        std::cout << "HIT! Puntuacion: " << _score << std::endl;
                    }
                    else {
                        _lives -= 1;
                        std::cout << "COLOR INCORRECTO! Vidas: " << _lives << std::endl;

                         if(t->getTargetType() == TargetType::NONE)
                            std::cout << "Color target: None" << std::endl;
                         if (p->getTargetType() == TargetType::NONE)
                            std::cout << "Color projectile: None" << std::endl;
                    }
                    p->setLifetime(0);
                    t->setLifetime(0);
                    break;
                }
            }
            ++it_targ;
        }
    }

    //7 LÓGICA DE FALLO y LIMPIEZA DE OBJETIVOS
    auto it_targ = targets.begin();
    while (it_targ != targets.end()) {
        Particle* t = *it_targ;

        bool removed = false;
        if (t->isAlive()) {
            if (t->getPos().z > 5.0f) {
                t->setLifetime(0);
                _lives -= 1;
                std::cout << "FALLO! Vidas: " << _lives << std::endl;
            }
        }

        //Si la partícula está muerta (por colisión / por fallo / por lifetime)
        if (!t->isAlive()) {
            _force_registry->remove(t, _target_mover);
            _force_registry->remove(t, _side_wind);
            _force_registry->remove(t, _bomb);
            _force_registry->remove(t, _target_drag);
            delete t;
            it_targ = targets.erase(it_targ);
            removed = true;
        }

        if (!removed) {
            ++it_targ;
        }
    }

    //8 Lógica de Muerte
    if (_lives <= 0) {
        std::cout << "--- GAME OVER --- Puntuacion Final: " << _score << std::endl;
        cleanup();
    }
}

void Scene5::cleanup() {
    //Restaurar cámara
    Camera* cam = GetCamera();
    if (cam) {
        cam->setEye(_original_cam_eye);
        cam->setDir(_original_cam_dir);
    }

    //Limpiar proyectiles
    for (auto p : _projectiles) {
        if (_force_registry) _force_registry->remove(p, _projectile_gravity);
        delete p;
    }
    _projectiles.clear();

    //Limpiar sistema de partículas (esto borra los objetivos y los generadores)
    if (_target_spawner) {
        delete _target_spawner;
        _target_spawner = nullptr;
    }

    //Limpiar fuerzas
    if (_force_registry) {
        _force_registry->clear();
        delete _force_registry;
        _force_registry = nullptr;
    }

    //Borrar generadores de fuerza
    delete _projectile_gravity;
    _projectile_gravity = nullptr;
    delete _target_mover;
    _target_mover = nullptr;
    delete _side_wind;
    _side_wind = nullptr;
    delete _bomb;
    _bomb = nullptr;
    delete _target_drag;
    _target_drag = nullptr;

    _red_spawner = nullptr;
    _blue_spawner = nullptr;
}

void Scene5::handleKeyPress(unsigned char key) {
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
        if (_bomb && _bomb_timer >= _bomb_cooldown) {
            std::cout << "¡BOMBA ACTIVADA!" << std::endl;
            _bomb->trigger();
            _bomb_timer = 0.0;
        }
        else {
            std::cout << "Bomba recargando... (" << static_cast<int>(_bomb_cooldown - _bomb_timer) << "s restantes)" << std::endl;
        }
        break;
    }
}

void Scene5::createProjectile(int type) {
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
    else { // ROJO (tipo 2)
        p = new LaserPistol(pos, dir * 250.0f);
        p->setColor({ 1.0f, 0.2f, 0.2f, 1.0f });
        p->setTargetType(TargetType::RED);
    }

    p->setupVisual();
    _projectiles.push_back(p);

}