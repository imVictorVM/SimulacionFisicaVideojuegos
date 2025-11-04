#pragma once
#include "Particle.h"

class Projectile : public Particle
{
public:
    Projectile(Vector3 pos, Vector3 vel, double mass, double dmp, double lifetime);

    virtual ~Projectile() {}

};