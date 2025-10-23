#include "Particle.h"
#include <cmath>
using namespace physx;


Particle::Particle(Vector3 p, Vector3 v, Vector3 a, double d, double life) :
    vel(v), acel(a), dmp(d), lifetime(life), renderItem(nullptr), color(1.0, 1.0, 1.0, 1.0)
{
    pose = new PxTransform(p);
}


Particle::Particle(const Particle& other) :
    vel(other.vel), acel(other.acel), dmp(other.dmp),
    lifetime(other.lifetime), renderItem(nullptr), color(other.color)
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
    return new Particle(*this);
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

void Particle::setColor(const Vector4& newColor)
{
    color = newColor;   
    if (renderItem) {
        renderItem->color = color;
    }
}

Vector3 Particle::getPos()
{
    return pose->p;
}

void Particle::setupVisual()
{
    if (renderItem == nullptr) {
        PxShape* shShape = CreateShape(PxSphereGeometry(0.1f));
        renderItem = new RenderItem(shShape, pose, color);
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