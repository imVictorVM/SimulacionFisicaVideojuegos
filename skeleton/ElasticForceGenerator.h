#pragma once
#include "SpringForceGenerator.h"

class ElasticForceGenerator : public SpringForceGenerator
{
public:
    ElasticForceGenerator(Particle* other_particle, float k, float resting_length);
    virtual ~ElasticForceGenerator() {}

    virtual void updateForce(Particle* particle, double t) override;
};