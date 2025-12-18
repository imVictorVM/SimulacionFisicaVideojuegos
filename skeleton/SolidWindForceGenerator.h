#pragma once
#include "SolidForceGenerator.h"

class SolidWindForceGenerator : public SolidForceGenerator
{
public:
    SolidWindForceGenerator(const Vector3& force);
    virtual ~SolidWindForceGenerator() {}

    virtual void updateForce(RigidBody* solid, float t) override;

protected:
    Vector3 _force;
};