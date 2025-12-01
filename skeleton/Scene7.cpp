#include "Scene7.h"
#include "RenderUtils.hpp"
#include "Render/Camera.h"
#include <iostream>

extern PxPhysics* gPhysics;
extern PxScene* gScene;

Scene7::Scene7() :
    _rb_registry(nullptr),
    _solid_generator(nullptr),
    _explosion(nullptr),
    _material(nullptr),
    _spawn_timer(0.0),
    _spawn_interval(0.2)
{
}

Scene7::~Scene7()
{
    cleanup();
}

void Scene7::createStaticFloor()
{
    PxRigidStatic* floor = gPhysics->createRigidStatic(PxTransform(Vector3(0, -1, 0)));
        PxShape* shape = CreateShape(PxBoxGeometry(100.0f, 1.0f, 100.0f), _material);
    floor->attachShape(*shape);
    gScene->addActor(*floor);

        _static_visuals.push_back(new RenderItem(shape, floor, { 0.5f, 0.5f, 0.5f, 1.0f }));
    _static_actors.push_back(floor);
}

void Scene7::createStaticObstacles()
{
    //Obstáculo 1: Rampa
    PxTransform transform({ 0.0f, 5.0f, 0.0f }, PxQuat(PxPi / 6.0f, { 0,0,1 }));
    PxRigidStatic* ramp = gPhysics->createRigidStatic(transform);
    PxShape* shape = CreateShape(PxBoxGeometry(10.0f, 1.0f, 10.0f), _material);
    ramp->attachShape(*shape);
    gScene->addActor(*ramp);

    _static_visuals.push_back(new RenderItem(shape, ramp, { 0.5f, 0.6f, 0.5f, 1.0f }));
    _static_actors.push_back(ramp);
}

//Actividad 1: Calcular tensor de inercia manualmente
void Scene7::spawnManualInertiaBox()
{
    std::cout << "Lanzando caja con tensor de inercia manual..." << std::endl;
    float w = 4, h = 1, d = 1; //Dimensiones
    float m = 10.0f; //Masa
    Vector3 halfExtents(w / 2, h / 2, d / 2);

    RigidBody* rb = new RigidBody(gPhysics, gScene, { 10, 20, 0 }, PxBoxGeometry(halfExtents), _material, 20.0f);
    rb->setVelocity({ 0,0,-10 });
    rb->setColor({ 1.0f, 0.0f, 1.0f, 1.0f });

    //Calcular tensor de inercia para un paralelepípedo
    float Ixx = (1.0f / 12.0f) * m * (h * h + d * d);
    float Iyy = (1.0f / 12.0f) * m * (w * w + d * d);
    float Izz = (1.0f / 12.0f) * m * (w * w + h * h);

    PxVec3 inertiaTensor(Ixx, Iyy, Izz);

    rb->getActor()->setMass(m);
    rb->getActor()->setMassSpaceInertiaTensor(inertiaTensor);

        _solids.push_back(rb);
}

void Scene7::shoot()
{
    Camera* cam = GetCamera();
    if (!cam) return;

    Vector3 pos = cam->getEye();
    Vector3 dir = cam->getDir();


    float w = 1, h = 1, d = 1; //Dimensiones
    float m = 10.0f; //Masa
    Vector3 halfExtents(w / 2, h / 2, d / 2);

    RigidBody* rb = new RigidBody(gPhysics, gScene, pos, PxBoxGeometry(halfExtents), _material, 20.0f);
    rb->setVelocity(dir * 50);
    rb->setColor({ 0.0f, 0.0f, 1.0f, 1.0f });

    //Calcular tensor de inercia para un paralelepípedo
    float Ixx = (1.0f / 12.0f) * m * (h * h + d * d);
    float Iyy = (1.0f / 12.0f) * m * (w * w + d * d);
    float Izz = (1.0f / 12.0f) * m * (w * w + h * h);

    PxVec3 inertiaTensor(Ixx, Iyy, Izz);

    rb->getActor()->setMass(m);
    rb->getActor()->setMassSpaceInertiaTensor(inertiaTensor);

    _solids.push_back(rb);
}


void Scene7::initialize()
{
    std::cout << "Inicializando " << getDescription() << std::endl;
    std::cout << " 'G': Lanzar solidos (Generador)" << std::endl;
    std::cout << " 'H': Lanzar caja con inercia manual" << std::endl;
    std::cout << " 'E': Detonar explosion" << std::endl;

    _rb_registry = new RigidBodyForceRegistry();

    // Crear material
    // Fricción estática=0.5, Fricción dinámica=0.5, Rebote=0.6
    _material = gPhysics->createMaterial(0.5f, 0.8f, 0.2f);

    //Crear suelo y obstáculos
    createStaticFloor();
    createStaticObstacles();

    //Configurar generador de sólidos
    _solid_generator = new GaussianSolidGenerator("Cajas", gPhysics, gScene);
    _solid_generator->setMeanPosition({ 0, 20, 0 });
    _solid_generator->setMeanVelocity({ 0, 0, 0 });
    _solid_generator->setVelDeviation({ 2, 0, 2 });
    _solid_generator->setBoxHalfExtents({ 0.5f, 0.5f, 0.5f });
    _solid_generator->setMaterial(_material);
    _solid_generator->setNumObjects(1);
    _solid_generator->setActive(false);

    //Configurar generador de explosión
    _explosion = new ExplosionSolidForceGenerator(20000.0, 0.5, 50.0, { 0, 10, 0 });
}

void Scene7::update(double t)
{
    if (!_rb_registry) return;

   

    //1 Generar nuevos sólidos
    if (_solid_generator) {
        _spawn_timer += t;
        // Solo generamos si ha pasado el tiempo suficiente
        if (_spawn_timer >= _spawn_interval) {
            _spawn_timer = 0.0;

            auto new_solids = _solid_generator->generateSolids();
            for (auto s : new_solids) {
                _solids.push_back(s);
                _rb_registry->add(s, _explosion);
            }
        }
    }

    //2 Actualizar fuerzas
    _rb_registry->updateForces(t);

    //Actualizar tiempo explosión
    if (_explosion) {
        _explosion->update(t);
    }

    //3 Integrar y limpiar sólidos
    auto it = _solids.begin();
    while (it != _solids.end()) {
        RigidBody* rb = *it;
        rb->update(t); //Actualiza lifetime

        if (!rb->isAlive()) {
            _rb_registry->remove(rb, _explosion); //Quitar del registro
            delete rb;
            it = _solids.erase(it);
        }
        else {
            ++it;
        }
    }
}

void Scene7::cleanup()
{
    for (auto s : _solids) {
        delete s;
    }
    _solids.clear();

    delete _rb_registry; _rb_registry = nullptr;
    delete _solid_generator; _solid_generator = nullptr;
    delete _explosion; _explosion = nullptr;

    for (auto v : _static_visuals) {
        v->release();
    }
    _static_visuals.clear();

    //Los actores estáticos son liberados por la escena
    _static_actors.clear();

    if (_material) {
        _material->release();
        _material = nullptr;
    }
}

void Scene7::handleKeyPress(unsigned char key)
{
    switch (toupper(key))
    {
    case 'G':
        if (_solid_generator) _solid_generator->toggleActive();
        break;
    case 'H':
        spawnManualInertiaBox();
        break;
    case 'E':
        if (_explosion) _explosion->trigger();
            std::cout << "¡EXPLOSION!" << std::endl;
        break;

    case 'P':
        shoot();
        break;
    }
}