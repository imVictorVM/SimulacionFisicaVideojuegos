#pragma once
#include "RigidBody.h"

class SolidForceGenerator
{
public:
    virtual ~SolidForceGenerator() {}
    virtual void updateForce(RigidBody* solid, float t) = 0;
};