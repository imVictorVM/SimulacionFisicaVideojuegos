#include "Projectile.h"
#include <cmath>
using namespace physx;

Projectile::Projectile(Vector3 p, Vector3 v, Vector3 a, double d, double m, double life)
    : Particle(p, v, a, d), mass(m), lifetime(life), initialLife(life)
{
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