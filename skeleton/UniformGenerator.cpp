#include "UniformGenerator.h"

UniformGenerator::UniformGenerator(std::string name) :
    ParticleGenerator(name),
    min_pos(-1.0, -1.0, -1.0), 
    max_pos(1.0, 1.0, 1.0),
    min_vel(-1.0, -1.0, -1.0), 
    max_vel(1.0, 1.0, 1.0)
{
}

UniformGenerator::~UniformGenerator() {
}

void UniformGenerator::setPosRange(const Vector3& min, const Vector3& max) {
    min_pos = min;
    max_pos = max;
}

void UniformGenerator::setVelRange(const Vector3& min, const Vector3& max) {
    min_vel = min;
    max_vel = max;
}

std::list<Particle*> UniformGenerator::generateParticles() {
    std::list<Particle*> new_particles;

    if (!is_active) return new_particles;

    for (int i = 0; i < num_particles_per_generation; ++i) {
        //1 clona la partícula modelo
        Particle* p = model_particle->clone();

        //2 calcula la nueva posición con distribución uniforme
        float rand_x = (float)uniform_dist(_mt); 
        float rand_y = (float)uniform_dist(_mt);
        float rand_z = (float)uniform_dist(_mt);
        Vector3 new_pos;
        new_pos.x = min_pos.x + rand_x * (max_pos.x - min_pos.x);
        new_pos.y = min_pos.y + rand_y * (max_pos.y - min_pos.y);
        new_pos.z = min_pos.z + rand_z * (max_pos.z - min_pos.z);
        p->setPosition(new_pos);

        //3 calcula la nueva velocidad con distribución uniforme
        rand_x = (float)uniform_dist(_mt);
        rand_y = (float)uniform_dist(_mt);
        rand_z = (float)uniform_dist(_mt);
        Vector3 new_vel;
        new_vel.x = min_vel.x + rand_x * (max_vel.x - min_vel.x);
        new_vel.y = min_vel.y + rand_y * (max_vel.y - min_vel.y);
        new_vel.z = min_vel.z + rand_z * (max_vel.z - min_vel.z);
        p->setVelocity(new_vel);

        //4 añade la nueva partícula a la lista
        new_particles.push_back(p);
    }

    return new_particles;
}