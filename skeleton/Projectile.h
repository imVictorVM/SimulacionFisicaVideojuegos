#pragma once
#include "Particle.h"

class Projectile : public Particle
{
public:
   
    Projectile(Vector3 pos, Vector3 simVel, //datos de la simulación
        double realMass, double realVelocity, //datos del proyectil real
        double dmp, double lifetime);

    virtual ~Projectile() {}

    virtual void integrate(double t) override;
    double getKineticEnergy();
    bool isAlive();
    virtual void setupVisual() override;

private:
    static double calculateSimMass(double realMass, double realVelocity, double simVelocityMag);
    static Vector3 calculateSimAcceleration(double realVelocity, double simVelocityMag);

protected:
    double mass;
    double lifetime;
    double initialLife;
};