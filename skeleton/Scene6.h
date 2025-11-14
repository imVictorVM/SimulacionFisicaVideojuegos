#pragma once
#include "Scene.h"
#include "Particle.h"
#include "BoxParticle.h"
#include "ParticleForceRegistry.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"
#include "AnchoredSpringForceGenerator.h"
#include "SpringForceGenerator.h"
#include "ElasticForceGenerator.h"
#include "BuoyancyForceGenerator.h"
#include <vector>

class Scene6 : public Scene {
public:
    Scene6();
    virtual ~Scene6();

    void initialize() override;
    void update(double t) override;
    void cleanup() override;
    void handleKeyPress(unsigned char key) override;
    std::string getDescription() const override { return "Escena 6: Muelles y Flotacion"; }

private:
    ParticleForceRegistry* _registry;
    GravityForceGenerator* _gravity;

    //1 Muelle anclado
    Particle* _anchored_particle;
    AnchoredSpringForceGenerator* _anchored_spring;
    Vector3 _anchor_pos;
    float _k_anchored;

    //2 Muelle y Elástico
    Particle* _p1_spring;
    Particle* _p2_spring;
    SpringForceGenerator* _spring;

    Particle* _p1_elastic;
    Particle* _p2_elastic;
    ElasticForceGenerator* _elastic;

    //3 Flotación
    BoxParticle* _buoyancy_box;
    BuoyancyForceGenerator* _buoyancy_force;
    RenderItem* _water_surface;
    physx::PxTransform* _water_transform;
    float _box_mass;
    float _box_radius;


    std::vector<Particle*> _particles;
};