#pragma once
#include "SolidForceGenerator.h"

class ExplosionSolidForceGenerator : public SolidForceGenerator
{
public:
    ExplosionSolidForceGenerator(double k, double t_constant, double radius, const Vector3& center);

    virtual void updateForce(RigidBody* solid, float t) override;

    void trigger() {
        _is_active = true;
        _time_elapsed = 0.0;
    }

protected:
    double _K;
    double _T;
    double _R;
    Vector3 _center;
    bool _is_active;
    double _time_elapsed;
};