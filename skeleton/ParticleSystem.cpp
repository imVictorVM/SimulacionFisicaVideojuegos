#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() {
}

ParticleSystem::~ParticleSystem() {
    cleanup();
}

void ParticleSystem::addGenerator(ParticleGenerator* generator) {
    _generators.push_back(generator);
}

std::list<Particle*> ParticleSystem::update(double t) {
    std::list<Particle*> newly_created_particles;

    //1 Generación de nuevas partículas
    for (auto gen : _generators) {
        std::list<Particle*> new_particles_from_gen = gen->generateParticles();
        for (auto p : new_particles_from_gen) {
            p->setupVisual();
            _particles.push_back(p);
            newly_created_particles.push_back(p);
        }
    }

    //2 Mantenimiento de partículas existentes
    for (auto p : _particles) {
        if (p->isAlive()) {
            p->integrate(t);
        }
    }

    return newly_created_particles;
}

std::list<Particle*>& ParticleSystem::getParticles() {
    return _particles;
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