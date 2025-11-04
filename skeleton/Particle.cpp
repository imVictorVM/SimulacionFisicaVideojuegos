#include "Particle.h"
#include <cmath>
using namespace physx;


Particle::Particle(Vector3 p, Vector3 v, double mass, double d, double life) :
    vel(v),
    _mass((float)mass),
    _dmp((float)d),
    _lifetime((float)life),
    renderItem(nullptr),
    color(1.0f, 1.0f, 1.0f, 1.0f), 
    _force_accumulator(0.0f)
{
    pose = new PxTransform(p);

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
    _dmp(other._dmp),
    _lifetime(other._lifetime),
    color(other.color),
    renderItem(nullptr),
    _force_accumulator(0.0f)
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
    _dmp = other._dmp;
    _lifetime = other._lifetime;
    color = other.color;
    *pose = *other.pose;
    return *this;
}


void Particle::integrate(double t)
{
    float dt = (float)t;

    //1 Calcular aceleración (solo si la masa es finita)
    Vector3 resulting_acel(0.0f);
    if (_inverse_mass > 0.0f) {
        resulting_acel = _force_accumulator * _inverse_mass;
    }

    //2 Actualizar velocidad
    vel += resulting_acel * dt;
    vel *= std::pow(_dmp, dt); 

    //3 Actualizar posición
    pose->p += vel * dt;

    //4 Limpiar fuerzas
    clearForce();

    
    _lifetime -= dt;
}

void Particle::addForce(const Vector3& force)
{
    _force_accumulator += force;
}

void Particle::clearForce()
{
    _force_accumulator = Vector3(0.0f);
}


double Particle::getMass() const
{
    return (double)_mass;
}

double Particle::getInverseMass() const
{
    return (double)_inverse_mass;
}

Vector3 Particle::getVelocity() const
{
    return vel;
}

bool Particle::isAlive() const
{
    return _lifetime > 0.0f;
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
    _lifetime = (float)life;
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