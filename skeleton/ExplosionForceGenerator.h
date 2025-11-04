#pragma once
#include "ForceGenerator.h"

class ExplosionForceGenerator : public ForceGenerator
{
public:
    ExplosionForceGenerator(double k, double t_constant, double radius, const Vector3& center);

    virtual void updateForce(Particle* particle, double t) override;

    void trigger();

    void setCenter(const Vector3& center);

protected:
    double _K;
    double _T;
    double _R;
    Vector3 _center;

    bool _is_active;
    double _time_elapsed; 
};