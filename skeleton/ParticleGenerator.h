#pragma once
#include "Particle.h"
#include <list>
#include <random>
#include <string>

class ParticleGenerator {
public:
    ParticleGenerator(std::string name);
    virtual ~ParticleGenerator();

    virtual std::list<Particle*> generateParticles() = 0;

    void setParticleModel(Particle* model);
    void setMeanPosition(const Vector3& pos);
    void setMeanVelocity(const Vector3& vel);
    void setNumParticles(int num);

    void setActive(bool active);
    bool isActive() const;
    void toggleActive();

protected:
    std::string name;
    Particle* model_particle; 

    Vector3 mean_pos;
    Vector3 mean_vel;

    int num_particles_per_generation = 1;

    bool is_active = true;

    std::mt19937 _mt;
    std::uniform_real_distribution<double> uniform_dist;
};