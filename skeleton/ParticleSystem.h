#pragma once
#include "Particle.h"
#include "ParticleGenerator.h"
#include <list>

class ParticleSystem {
public:
    ParticleSystem();
    ~ParticleSystem();

    virtual std::list<Particle*> update(double t);

    std::list<Particle*>& getParticles();

    void addGenerator(ParticleGenerator* generator);

    void cleanup();

private:
    std::list<Particle*> _particles;
    std::list<ParticleGenerator*> _generators;
};