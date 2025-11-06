#include "BoxParticle.h"
using namespace physx;

BoxParticle::BoxParticle(Vector3 Pos, Vector3 Vel, double mass, double dmp, double lifetime)
    : Particle(Pos, Vel, mass, dmp, lifetime)
{
}

void BoxParticle::setupVisual()
{
    if (renderItem == nullptr) {
        PxShape* shShape = CreateShape(PxBoxGeometry(_radius, _radius, _radius));
        renderItem = new RenderItem(shShape, pose, color);
    }
}

Particle* BoxParticle::clone() const
{
    return new BoxParticle(*this);
}