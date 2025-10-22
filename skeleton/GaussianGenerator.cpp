#include "GaussianGenerator.h"

GaussianGenerator::GaussianGenerator(std::string name) :
    ParticleGenerator(name),
    pos_deviation(1.0, 1.0, 1.0), 
    vel_deviation(1.0, 1.0, 1.0),
    normal_dist(0.0, 1.0) 
{
}

GaussianGenerator::~GaussianGenerator() {
}

void GaussianGenerator::setPosDeviation(const Vector3& deviation) {
    pos_deviation = deviation;
}

void GaussianGenerator::setVelDeviation(const Vector3& deviation) {
    vel_deviation = deviation;
}

std::list<Particle*> GaussianGenerator::generateParticles() {
    std::list<Particle*> new_particles;

    for (int i = 0; i < num_particles_per_generation; ++i) {
        //1 clona la partícula modelo
        Particle* p = model_particle->clone();

        //2 calcula la nueva posición
        Vector3 new_pos = mean_pos;
        new_pos.x += normal_dist(_mt) * pos_deviation.x;
        new_pos.y += normal_dist(_mt) * pos_deviation.y;
        new_pos.z += normal_dist(_mt) * pos_deviation.z;
        p->setPosition(new_pos);

        //3 calcula la nueva velocidad 
        Vector3 new_vel = mean_vel;
        new_vel.x += normal_dist(_mt) * vel_deviation.x;
        new_vel.y += normal_dist(_mt) * vel_deviation.y;
        new_vel.z += normal_dist(_mt) * vel_deviation.z;
        p->setVelocity(new_vel);

        //4 añade la nueva partícula a la lista
        new_particles.push_back(p);
    }

    return new_particles;
}