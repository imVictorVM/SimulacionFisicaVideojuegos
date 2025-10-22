#pragma once
#include "ParticleGenerator.h"

class UniformGenerator : public ParticleGenerator {
public:
    UniformGenerator(std::string name);
    virtual ~UniformGenerator();

    // Implementación específica del método de generación
    virtual std::list<Particle*> generateParticles() override;

    // Métodos para configurar los rangos
    void setPosRange(const Vector3& min_pos, const Vector3& max_pos);
    void setVelRange(const Vector3& min_vel, const Vector3& max_vel);

protected:
    Vector3 min_pos;
    Vector3 max_pos;
    Vector3 min_vel;
    Vector3 max_vel;
};