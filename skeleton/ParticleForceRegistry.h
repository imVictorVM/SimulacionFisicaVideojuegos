#pragma once
#include "Particle.h"
#include "ForceGenerator.h"
#include <vector>

class ParticleForceRegistry {
protected:
    struct ParticleForceRegistration {
        Particle* particle;
        ForceGenerator* fg;
    };

    std::vector<ParticleForceRegistration> _registry;

public:
    void add(Particle* particle, ForceGenerator* fg);
    void remove(Particle* particle, ForceGenerator* fg);
    void clear();
    void updateForces(double t);
};