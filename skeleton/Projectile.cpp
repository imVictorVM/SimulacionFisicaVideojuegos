#include "Projectile.h"
#include <cmath>
using namespace physx;

Projectile::Projectile(Vector3 pos, Vector3 simVel,
    double realMass, double realVelocity,
    double dmp, double life)
    : Particle(pos, simVel,
        calculateSimAcceleration(realVelocity, simVel.magnitude()), dmp),
    mass(calculateSimMass(realMass, realVelocity, simVel.magnitude())), 
    lifetime(life), initialLife(life)
{
}


double Projectile::calculateSimMass(double realMass, double realVelocity, double simVelocityMag) {
    if (simVelocityMag == 0) return realMass; 
    const double kineticEnergy = 0.5 * realMass * pow(realVelocity, 2);
    return (2.0 * kineticEnergy) / pow(simVelocityMag, 2);
}

Vector3 Projectile::calculateSimAcceleration(double realVelocity, double simVelocityMag) {
    if (realVelocity == 0) return Vector3(0);
    const double realGravity = -9.8;
    const double velocityRatio = simVelocityMag / realVelocity;
    const double simGravity = realGravity * pow(velocityRatio, 2);
    return Vector3(0, simGravity, 0);
}


void Projectile::integrate(double t)
{
    Particle::integrate(t);
    lifetime -= t;
}

double Projectile::getKineticEnergy()
{
    return 0.5 * mass * vel.magnitudeSquared();
}

bool Projectile::isAlive()
{
    return lifetime > 0;
}

void Projectile::setupVisual()
{
    if (renderItem == nullptr) {
        PxShape* shShape = CreateShape(PxSphereGeometry(1.0f));
        renderItem = new RenderItem(shShape, pose, Vector4(1, 1, 1, 1));
    }
}