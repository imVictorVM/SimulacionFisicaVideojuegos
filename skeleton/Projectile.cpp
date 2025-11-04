#include "Projectile.h"
#include <cmath>
using namespace physx;

Projectile::Projectile(Vector3 pos, Vector3 vel, double mass, double dmp, double life)
    : Particle(pos, vel, mass, dmp, life)
{
}