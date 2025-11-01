#pragma once
#include "Scene.h"
#include <vector>

class Particle;
class ParticleForceRegistry;
class GravityForceGenerator;
class WindForceGenerator;

class Scene3 : public Scene {
public:
    Scene3();
    virtual ~Scene3();

    void initialize() override;
    void update(double t) override;
    void cleanup() override;
    void handleKeyPress(unsigned char key) override;
    std::string getDescription() const override { return "Escena 3: Fuerzas"; }

private:
    std::vector<Particle*> _particles;

    ParticleForceRegistry* _force_registry;
    GravityForceGenerator* _gravity_generator;
    WindForceGenerator* _wind_generator;
};