#include "Particle.h"
#include <cmath>
using namespace physx;


Particle::Particle(Vector3 p, Vector3 v, Vector3 a, double d, double life) :
    vel(v), acel(a), dmp(d), lifetime(life), renderItem(nullptr)
{
    pose = new PxTransform(p);
}


Particle::Particle(const Particle& other) :
    vel(other.vel), acel(other.acel), dmp(other.dmp), lifetime(other.lifetime), renderItem(nullptr)
{
    pose = new PxTransform(other.pose->p);
    
}

Particle::~Particle()
{
    cleanup();
}


Particle& Particle::operator=(const Particle& other)
{
    if (this == &other) return *this;

    vel = other.vel;
    acel = other.acel;
    dmp = other.dmp;
    lifetime = other.lifetime;
    *pose = *other.pose; 

    return *this;
}

void Particle::integrate(double t)
{
    // EULER EXPLICITO
    // pose->p = pose->p + (t * vel);
    // vel = vel + (t * acel);

    // Euler semi-implícito
    vel += acel * t;
    vel *= pow(dmp, t);
    pose->p += vel * t;

    // VERLET 
   // Vector3 newPos = 2.0f * pose->p - prePos + acel * (t * t);
   // prePos = pose->p;
   // pose->p = newPos;

    lifetime -= t;
}


bool Particle::isAlive() const
{
    return lifetime > 0.0;
}

Particle* Particle::clone() const
{
    return new Particle(pose->p, vel, acel, dmp, lifetime);
}

void Particle::setPosition(const Vector3& pos) {
    if (pose) pose->p = pos;
}

void Particle::setVelocity(const Vector3& v) {
    vel = v;
}

void Particle::setLifetime(double life) {
    lifetime = life;
}


Vector3 Particle::getPos()
{
    return pose->p;
}

void Particle::setupVisual()
{
    if (renderItem == nullptr) {
        PxShape* shShape = CreateShape(PxSphereGeometry(0.1f));
        renderItem = new RenderItem(shShape, pose, Vector4(1, 1, 1, 1));
    }
}

void Particle::cleanup()
{
    if (renderItem) {
        renderItem->release();
        renderItem = nullptr;
    }
    if (pose) {
        delete pose;
        pose = nullptr;
    }
}