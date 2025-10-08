#pragma once
#include "Particle.h"

class Projectile : public Particle
{
public:
    Projectile(Vector3 Pos, Vector3 Vel, Vector3 Acel, double dmp, double mass, double lifetime);
    virtual ~Projectile() {}

    virtual void integrate(double t) override;
    double getKineticEnergy();
    bool isAlive();
    virtual void setupVisual() override;

protected:
    double mass;
    double lifetime;
    double initialLife;
};
