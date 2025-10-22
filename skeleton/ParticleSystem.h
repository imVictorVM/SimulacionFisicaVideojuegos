#pragma once
#include "Particle.h"
#include "ParticleGenerator.h"
#include <list>

class ParticleSystem {
public:
    ParticleSystem();
    ~ParticleSystem();

    void update(double t);

    void addGenerator(ParticleGenerator* generator);


    void cleanup();

private:
    std::list<Particle*> _particles;
    std::list<ParticleGenerator*> _generators;
};