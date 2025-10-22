#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() {
}

ParticleSystem::~ParticleSystem() {
    cleanup();
}

void ParticleSystem::addGenerator(ParticleGenerator* generator) {
    _generators.push_back(generator);
}

void ParticleSystem::update(double t) {
    //1 generación de nuevas partículas
    for (auto gen : _generators) {
        std::list<Particle*> new_particles = gen->generateParticles();
        for (auto p : new_particles) {
            p->setupVisual(); 
            _particles.push_back(p);
        }
    }

    //2 mantenimiento de partículas existentes
    auto it = _particles.begin();
    while (it != _particles.end()) {
        Particle* p = *it;

        if (!p->isAlive()) {
            delete p;
            it = _particles.erase(it);
        }
        else {
            p->integrate(t);
            ++it;
        }
    }
}

void ParticleSystem::cleanup() {
    for (auto p : _particles) {
        delete p;
    }
    _particles.clear();

    for (auto g : _generators) {
        delete g;
    }
    _generators.clear();
}