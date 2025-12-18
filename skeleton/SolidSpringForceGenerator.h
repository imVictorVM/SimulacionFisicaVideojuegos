#pragma once
#include "SolidForceGenerator.h"
#include "RenderUtils.hpp"

class SolidAnchoredSpringForceGenerator : public SolidForceGenerator
{
public:
    SolidAnchoredSpringForceGenerator(Vector3* anchor_pos, float k, float resting_length);
    virtual ~SolidAnchoredSpringForceGenerator();

    virtual void updateForce(RigidBody* solid, float t) override;
    void setupVisual(PxShape* shape);

protected:
    Vector3* _anchor_pos;
    float _k;
    float _resting_length;
    RenderItem* _anchor_visual;
};