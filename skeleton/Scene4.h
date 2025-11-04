#pragma once
#include "Scene.h"
#include <vector>

class Particle;
class ParticleForceRegistry;
class GravityForceGenerator;
class ExplosionForceGenerator;

class Scene4 : public Scene {
public:
    Scene4();
    virtual ~Scene4();

    void initialize() override;
    void update(double t) override;
    void cleanup() override;
    void handleKeyPress(unsigned char key) override;
    std::string getDescription() const override { return "Escena 4: Explosion"; }

private:
    void createParticleBox();

    std::vector<Particle*> _particles;

    ParticleForceRegistry* _force_registry;
    GravityForceGenerator* _gravity_generator;
    ExplosionForceGenerator* _explosion_generator;
};