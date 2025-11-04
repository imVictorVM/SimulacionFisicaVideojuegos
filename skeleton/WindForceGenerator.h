#pragma once
#include "ForceGenerator.h"

class WindForceGenerator : public ForceGenerator
{
public:
    WindForceGenerator(const Vector3& wind_vel, float k1 = 1.0f, float k2 = 0.0f);
    virtual void updateForce(Particle* particle, double t) override;

    // Este método lo heredará y usará el torbellino
    void setWindArea(const Vector3& corner1, const Vector3& corner2);

protected:
    // NUEVO MÉTODO PROTEGIDO: Para que las clases hijas lo usen
    bool isParticleInArea(Particle* particle);

private:
    Vector3 _wind_velocity;
    float _k1, _k2;

    // Estas variables ahora son privadas, se accede a ellas vía isParticleInArea
    Vector3 _area_corner1, _area_corner2;
    bool _area_active;
};