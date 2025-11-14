#include "ElasticForceGenerator.h"

ElasticForceGenerator::ElasticForceGenerator(Particle* other_particle, float k, float resting_length) :
    SpringForceGenerator(other_particle, k, resting_length)
{
}

void ElasticForceGenerator::updateForce(Particle* particle, double t)
{
    Vector3 d = particle->getPos() - _other_particle->getPos();
    float l = d.magnitude();

    if (l > 1e-6)
    {
        //Solo aplica fuerza si se estira
        if (l > _resting_length)
        {
            float displacement = l - _resting_length;
            Vector3 force = d.getNormalized() * (-_k * displacement);
            particle->addForce(force);
        }
    }
}