#include "SolidWindForceGenerator.h"

SolidWindForceGenerator::SolidWindForceGenerator(const Vector3& force) :
    _force(force)
{
}

void SolidWindForceGenerator::updateForce(RigidBody* solid, float t)
{
    if (solid->isAlive()) {
        solid->addForce(_force);
    }
}