#pragma once
#include "ForceGenerator.h"

class GravityForceGenerator : public ForceGenerator
{
public:

    GravityForceGenerator(const Vector3& gravity = { 0.0, -9.8, 0.0 });

 
    virtual void updateForce(Particle* particle, double t) override;

    void setGravity(const Vector3& gravity) { _gravity = gravity; }
    Vector3 getGravity() const { return _gravity; }

protected:

    Vector3 _gravity;
};