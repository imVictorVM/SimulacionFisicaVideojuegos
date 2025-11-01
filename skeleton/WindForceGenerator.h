#pragma once
#include "ForceGenerator.h"

class WindForceGenerator : public ForceGenerator
{
public:
   
    WindForceGenerator(const Vector3& wind_vel, float k1 = 1.0f, float k2 = 0.0f);

  
    virtual void updateForce(Particle* particle, double t) override;


    void setWindArea(const Vector3& corner1, const Vector3& corner2);

protected:
    Vector3 _wind_velocity; 
    float _k1;
    float _k2;

   
    Vector3 _area_corner1;
    Vector3 _area_corner2;
    bool _area_active;
};