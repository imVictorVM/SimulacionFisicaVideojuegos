#pragma once
#include "ForceGenerator.h"

class BuoyancyForceGenerator : public ForceGenerator
{
public:
    BuoyancyForceGenerator(float water_height, float liquid_density);
    virtual ~BuoyancyForceGenerator() {}

    virtual void updateForce(Particle* particle, double t) override;

protected:
    float _water_height;
    float _liquid_density;
};