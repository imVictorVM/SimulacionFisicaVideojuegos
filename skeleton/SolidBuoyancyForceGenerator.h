#pragma once
#include "SolidForceGenerator.h"

class SolidBuoyancyForceGenerator : public SolidForceGenerator
{
public:
    SolidBuoyancyForceGenerator(float water_height, float liquid_density);
    virtual ~SolidBuoyancyForceGenerator() {}

    virtual void updateForce(RigidBody* solid, float t) override;

protected:
    float _water_height;
    float _liquid_density;
};