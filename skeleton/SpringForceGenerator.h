#pragma once
#include "ForceGenerator.h"

class SpringForceGenerator : public ForceGenerator
{
public:
    SpringForceGenerator(Particle* other_particle, float k, float resting_length);
    virtual ~SpringForceGenerator() {}

    virtual void updateForce(Particle* particle, double t) override;

    void setK(float k) { _k = k; }
    float getK() const { return _k; }

protected:
    Particle* _other_particle;
    float _k;
    float _resting_length;
};