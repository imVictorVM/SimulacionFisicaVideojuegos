#pragma once
#include "ForceGenerator.h"
#include "RenderUtils.hpp"

class AnchoredSpringForceGenerator : public ForceGenerator
{
public:
    AnchoredSpringForceGenerator(Vector3* anchor_pos, float k, float resting_length);
    virtual ~AnchoredSpringForceGenerator();

    virtual void updateForce(Particle* particle, double t) override;

    void setK(float k) { _k = k; }
    float getK() const { return _k; }

    void setupVisual(physx::PxShape* shape);

protected:
    Vector3* _anchor_pos;
    float _k;
    float _resting_length;

    RenderItem* _anchor_visual;
};