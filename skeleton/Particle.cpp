#include "Particle.h"
#include <cmath>
using namespace physx;

Particle::Particle(Vector3 p, Vector3 v, Vector3 a, double d)
{
    vel = v;
    pose = new PxTransform(p);
    acel = a;
    dmp = d;
 
    renderItem = nullptr;
}

Particle::~Particle()
{
    if (renderItem != nullptr) {
        DeregisterRenderItem(renderItem);
        delete renderItem;
    }
    delete pose;
}

void Particle::integrate(double t)
{
    // EULER EXPLÍCITO
    // pose->p = pose->p + (t * vel);
    // vel = vel + (t * acel);

    // EULER SEMI-IMPLÍCITO 
    vel = vel + (t * acel);
    vel = vel * pow(dmp, t); 
    pose->p = pose->p + (t * vel);

    // VERLET 
    // Vector3 newPos = 2.0f * pose->p - prePos + acel * (t * t);
    // prePos = pose->p;
    // pose->p = newPos;
}

Vector3 Particle::getPos()
{
    return pose->p;
}

void Particle::setupVisual()
{
    if (renderItem == nullptr) {
        PxShape* shShape = CreateShape(PxSphereGeometry(1.0f));
        renderItem = new RenderItem(shShape, pose, Vector4(1, 0, 0, 1));
    }
}