#include "Particle.h"
#include <cmath>
using namespace physx;

Particle::Particle(Vector3 p, Vector3 v, double mass, double d, double life) :
    vel(v),
    _mass(mass),
    dmp(d),
    lifetime(life),
    renderItem(nullptr),
    color(1.0, 1.0, 1.0, 1.0),
    _force_accumulator(0.0)
{
    pose = new PxTransform(p);

    // Calcula el inverso de la masa. Si la masa es 0, se considera masa infinita.
    if (_mass <= 0.0f) {
        _inverse_mass = 0.0f;
    }
    else {
        _inverse_mass = 1.0f / _mass;
    }
}

Particle::Particle(const Particle& other) :
    vel(other.vel),
    _mass(other._mass),
    _inverse_mass(other._inverse_mass),
    dmp(other.dmp),
    lifetime(other.lifetime),
    color(other.color),
    renderItem(nullptr),
    _force_accumulator(0.0)
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
    _mass = other._mass;
    _inverse_mass = other._inverse_mass;
    dmp = other.dmp;
    lifetime = other.lifetime;
    color = other.color;
    *pose = *other.pose;
    return *this;
}

void Particle::integrate(double t)
{
    // No integramos partículas con masa infinita.
    if (_inverse_mass <= 0.0f) return;

    Vector3 resulting_acel = _force_accumulator * _inverse_mass;

    vel += resulting_acel * t;
    vel *= pow(dmp, t);

    pose->p += vel * t;

    clearForce();

    lifetime -= t;
}

void Particle::addForce(const Vector3& force)
{
    _force_accumulator += force;
}

void Particle::clearForce()
{
    _force_accumulator = Vector3(0.0);
}

// --- GETTERS ---
double Particle::getMass() const
{
    return _mass;
}

double Particle::getInverseMass() const
{
    return _inverse_mass;
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
        PxShape* shShape = CreateShape(PxSphereGeometry(0.2f));
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