#pragma once
#include "WindForceGenerator.h" // <-- AHORA INCLUYE Y HEREDA DE WIND

class WhirlwindForceGenerator : public WindForceGenerator // <-- HERENCIA CAMBIADA
{
public:
    WhirlwindForceGenerator(float k, const Vector3& center, const Vector3& direction);

    // Sobrescribe el método del padre
    virtual void updateForce(Particle* particle, double t) override;

protected:
    float _K;
    Vector3 _center;
    Vector3 _direction;
};