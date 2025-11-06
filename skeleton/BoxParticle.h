#pragma once
#include "Particle.h"

class BoxParticle : public Particle
{
public:
    
    BoxParticle(Vector3 Pos, Vector3 Vel, double mass = 1.0, double dmp = 0.99, double lifetime = 1.0);

    virtual void setupVisual() override;

    virtual Particle* clone() const override;
};