#include "GravityForceGenerator.h"

GravityForceGenerator::GravityForceGenerator(const Vector3& gravity) :
    _gravity(gravity)
{
}

void GravityForceGenerator::updateForce(Particle* particle, double t)
{
    //No aplicar fuerza a partículas con masa infinita.
    if (particle->getInverseMass() == 0.0f) {
        return;
    }

    //Calcular la fuerza F = m * g y la añade al acumulador de la partícula.
    particle->addForce(_gravity * particle->getMass());
}