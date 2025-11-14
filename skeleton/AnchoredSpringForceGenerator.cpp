#include "AnchoredSpringForceGenerator.h"

using namespace physx;

AnchoredSpringForceGenerator::AnchoredSpringForceGenerator(Vector3* anchor_pos, float k, float resting_length) :
    _anchor_pos(anchor_pos),
    _k(k),
    _resting_length(resting_length),
    _anchor_visual(nullptr)
{
}

AnchoredSpringForceGenerator::~AnchoredSpringForceGenerator()
{
    if (_anchor_visual) {
        _anchor_visual->release();
        _anchor_visual = nullptr;
    }
}

void AnchoredSpringForceGenerator::setupVisual(PxShape* shape)
{
    if (_anchor_visual) _anchor_visual->release();
    //Creamos un PxTransform temporal (no se moverá)
    PxTransform* t = new PxTransform(*_anchor_pos);
    _anchor_visual = new RenderItem(shape, t, { 0.8f, 0.8f, 0.1f, 1.0f });
}

void AnchoredSpringForceGenerator::updateForce(Particle* particle, double t)
{
    Vector3 d = particle->getPos() - (*_anchor_pos);
    float l = d.magnitude();

    if (l > 1e-6) //Evitar división por cero
    {
        float displacement = l - _resting_length;
        Vector3 force = d.getNormalized() * (-_k * displacement);
        particle->addForce(force);
    }
}