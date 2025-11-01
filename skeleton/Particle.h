#pragma once
#include <vector>
#include "core.hpp"
#include "RenderUtils.hpp"

class Particle
{
public:
    Particle(Vector3 Pos, Vector3 Vel, double mass = 1.0, double dmp = 0.99, double lifetime = 10.0);
    Particle(const Particle& other);
    ~Particle();

    Particle& operator=(const Particle& other);

   
    virtual void integrate(double t);

    void addForce(const Vector3& force);
    void clearForce();

    
    double getMass() const;
    double getInverseMass() const;
    Vector3 getVelocity() const;


    void setPosition(const Vector3& pos);
    void setVelocity(const Vector3& vel);
    void setLifetime(double life);
    void setColor(const Vector4& color);
    Vector3 getPos();
    virtual void setupVisual();
    void cleanup();
    virtual bool isAlive() const;
    virtual Particle* clone() const;

protected:
    Vector3 vel;
    double _mass;
    double _inverse_mass;
    double dmp;
    Vector3 _force_accumulator;

    double lifetime;
    physx::PxTransform* pose;
    RenderItem* renderItem;
    Vector4 color;
};