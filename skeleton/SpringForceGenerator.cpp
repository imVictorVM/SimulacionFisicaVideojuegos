#include "SpringForceGenerator.h"

SpringForceGenerator::SpringForceGenerator(Particle* other_particle, float k, float resting_length) :
    _other_particle(other_particle),
    _k(k),
    _resting_length(resting_length)
{
}

void SpringForceGenerator::updateForce(Particle* particle, double t)
{
    Vector3 d = particle->getPos() - _other_particle->getPos();
    float l = d.magnitude();

    if (l > 1e-6) //Evitar división por cero
    {
        float displacement = l - _resting_length;
        Vector3 force = d.getNormalized() * (-_k * displacement);
        particle->addForce(force);
    }
}