#include "Scene6.h"
#include "RenderUtils.hpp"
#include "Render/Camera.h"
#include <iostream>

using namespace physx;

Scene6::Scene6() :
    _registry(nullptr),
    _gravity(nullptr),
    _anchored_particle(nullptr),
    _anchored_spring(nullptr),
    _anchor_pos(-15.0f, 15.0f, 0.0f),
    _k_anchored(100.0f),
    _p1_spring(nullptr),
    _p2_spring(nullptr),
    _spring(nullptr),
    _p1_elastic(nullptr),
    _p2_elastic(nullptr),
    _elastic(nullptr),
    _buoyancy_box(nullptr),
    _buoyancy_force(nullptr),
    _water_surface(nullptr),
    _water_transform(nullptr),
    _box_mass(10.0f),
    _box_radius(1.0f)
{
}

Scene6::~Scene6()
{
    cleanup();
}

void Scene6::initialize()
{
    std::cout << "Inicializando " << getDescription() << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    std::cout << " 'F': Aplicar fuerza al muelle anclado" << std::endl;
    std::cout << " 'K'/'J': Aumentar/Reducir K del muelle anclado (k=" << _k_anchored << ")" << std::endl;
    std::cout << " 'M'/'N': Aumentar/Reducir Masa del cubo (m=" << _box_mass << ")" << std::endl;
    std::cout << " 'R': Resetear escena" << std::endl;

    _registry = new ParticleForceRegistry();
    _gravity = new GravityForceGenerator({ 0.0f, -9.8f, 0.0f });

    //1 Muelle Anclado
    _anchored_particle = new Particle({ -15.0f, 10.0f, 0.0f }, { 0,0,0 }, 1.0, 0.9, 100.0);
    _anchored_particle->setupVisual();
    _particles.push_back(_anchored_particle);

    _anchored_spring = new AnchoredSpringForceGenerator(&_anchor_pos, _k_anchored, 5.0f);
    PxShape* anchor_shape = CreateShape(PxBoxGeometry(0.5f, 0.5f, 0.5f));
    _anchored_spring->setupVisual(anchor_shape);

    _registry->add(_anchored_particle, _gravity);
    _registry->add(_anchored_particle, _anchored_spring);

    //2 Muelle y Elástico
    //Muelle normal
    _p1_spring = new Particle({ -7.0f, 15.0f, 0.0f }, { 0,0,0 }, 1.0, 0.9, 100.0);
    _p2_spring = new Particle({ -3.0f, 10.0f, 0.0f }, { 0,0,0 }, 1.0, 0.9, 100.0);
    _p1_spring->setupVisual(); _p2_spring->setupVisual();
    _particles.push_back(_p1_spring); _particles.push_back(_p2_spring);

    _spring = new SpringForceGenerator(_p2_spring, 50.0f, 5.0f);
    _registry->add(_p1_spring, _gravity);
    _registry->add(_p1_spring, _spring); //p1 es afectado por p2

    SpringForceGenerator* spring_inv = new SpringForceGenerator(_p1_spring, 50.0f, 5.0f);
    _registry->add(_p2_spring, _gravity);
    _registry->add(_p2_spring, spring_inv); //p2 es afectado por p1

    //Goma elástica
    _p1_elastic = new Particle({ 7.0f, 15.0f, 0.0f }, { 0,0,0 }, 1.0, 0.9, 100.0);
    _p2_elastic = new Particle({ 3.0f, 10.0f, 0.0f }, { 0,0,0 }, 1.0, 0.9, 100.0);
    _p1_elastic->setupVisual(); _p2_elastic->setupVisual();
    _particles.push_back(_p1_elastic); _particles.push_back(_p2_elastic);

    _elastic = new ElasticForceGenerator(_p2_elastic, 50.0f, 5.0f);
    _registry->add(_p1_elastic, _gravity);
    _registry->add(_p1_elastic, _elastic); //p1 es afectado por p2

    ElasticForceGenerator* elastic_inv = new ElasticForceGenerator(_p1_elastic, 50.0f, 5.0f);
    _registry->add(_p2_elastic, _gravity);
    _registry->add(_p2_elastic, elastic_inv); //p2 es afectado por p1


    //3 Flotación
    _buoyancy_box = new BoxParticle({ 15.0f, 5.0f, 0.0f }, { 0,0,0 }, _box_mass, 0.9, 100.0);
    _buoyancy_box->setRadius(_box_radius);
    _buoyancy_box->setColor({ 0.6f, 0.4f, 0.2f, 1.0f });
    _buoyancy_box->setupVisual();
    _particles.push_back(_buoyancy_box);

    float water_height = 3.0f;
    _buoyancy_force = new BuoyancyForceGenerator(water_height, 1000.0f); //densidad del agua

    _registry->add(_buoyancy_box, _gravity);
    _registry->add(_buoyancy_box, _buoyancy_force);

    //Agua
    _water_transform = new PxTransform({ 15.0f, water_height, 0.0f });
    PxShape* water_shape = CreateShape(PxBoxGeometry(5.0f, 0.1f, 5.0f));
    _water_surface = new RenderItem(water_shape, _water_transform, { 0.2f, 0.5f, 1.0f, 0.4f });
}

void Scene6::update(double t)
{
    if (!_registry) return;


    _registry->updateForces(t);

    for (auto p : _particles)
    {
        if (p->isAlive()) {
            p->integrate(t);
        }
    }
}

void Scene6::cleanup()
{
    for (auto p : _particles) {
        delete p;
    }
    _particles.clear();

    delete _registry; _registry = nullptr;
    delete _gravity; _gravity = nullptr;
    delete _anchored_spring; _anchored_spring = nullptr;
    delete _spring; _spring = nullptr;
    delete _elastic; _elastic = nullptr;
    delete _buoyancy_force; _buoyancy_force = nullptr;


    if (_water_surface) {
        _water_surface->release();
        _water_surface = nullptr;
    }
    delete _water_transform; _water_transform = nullptr;
}

void Scene6::handleKeyPress(unsigned char key)
{
    switch (toupper(key))
    {
    case 'F':
        if (_anchored_particle)
            _anchored_particle->addForce({ 0, 0, 5000.0f });
        std::cout << "Fuerza aplicada al muelle anclado" << std::endl;
        break;
    case 'K':
        _k_anchored += 100.0f;
        if (_anchored_spring) _anchored_spring->setK(_k_anchored);
        std::cout << "Nueva k del muelle anclado: " << _k_anchored << std::endl;
        break;
    case 'J':
        _k_anchored = (std::max)(10.0f, _k_anchored - 100.0f);
        if (_anchored_spring) _anchored_spring->setK(_k_anchored);
        std::cout << "Nueva k del muelle anclado: " << _k_anchored << std::endl;
        break;
    case 'M':
        _box_mass += 5.0f;
        if (_buoyancy_box) _buoyancy_box->setMass(_box_mass);
        std::cout << "Nueva masa del cubo: " << _box_mass << std::endl;
        break;
    case 'N':
        _box_mass = (std::max)(1.0f, _box_mass - 5.0f);
        if (_buoyancy_box) _buoyancy_box->setMass(_box_mass);
        std::cout << "Nueva masa del cubo: " << _box_mass << std::endl;
        break;
    case 'R':
        cleanup();
        initialize();
        break;
    }
}