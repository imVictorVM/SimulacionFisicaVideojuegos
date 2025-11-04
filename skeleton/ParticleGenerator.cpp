#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(std::string name) :
    name(name),
    model_particle(nullptr),
    mean_pos(0.0),
    mean_vel(0.0, 10.0, 0.0),
    _mt(std::random_device{}()),
    uniform_dist(0.0, 1.0)
{
    //crear una partícula modelo por defecto
    setParticleModel(new Particle({ 0,0,0 }, { 0,0,0 }));
}

ParticleGenerator::~ParticleGenerator() {
    model_particle->cleanup();
    delete model_particle;
}

void ParticleGenerator::setParticleModel(Particle* model) {
    if (model_particle) {
        model_particle->cleanup();
        delete model_particle;
    }
    model_particle = model;
}

void ParticleGenerator::setMeanPosition(const Vector3& pos) {
    mean_pos = pos;
}

void ParticleGenerator::setMeanVelocity(const Vector3& vel) {
    mean_vel = vel;
}

void ParticleGenerator::setNumParticles(int num) {
    num_particles_per_generation = num;
}

void ParticleGenerator::setActive(bool active) {
    is_active = active;
}

bool ParticleGenerator::isActive() const {
    return is_active;
}

void ParticleGenerator::toggleActive() {
    is_active = !is_active;
}