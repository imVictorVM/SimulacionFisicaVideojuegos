#pragma once
#include "WindForceGenerator.h" 

class WhirlwindForceGenerator : public WindForceGenerator
{
public:
    WhirlwindForceGenerator(float k, const Vector3& center, const Vector3& direction);

    virtual void updateForce(Particle* particle, double t) override;

protected:
    float _K;
    Vector3 _center;
    Vector3 _direction;
};