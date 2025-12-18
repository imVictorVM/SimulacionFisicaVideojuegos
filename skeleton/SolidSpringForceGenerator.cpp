#include "SolidSpringForceGenerator.h"

SolidAnchoredSpringForceGenerator::SolidAnchoredSpringForceGenerator(Vector3* anchor_pos, float k, float resting_length) :
    _anchor_pos(anchor_pos), _k(k), _resting_length(resting_length), _anchor_visual(nullptr)
{
}

SolidAnchoredSpringForceGenerator::~SolidAnchoredSpringForceGenerator()
{
    if (_anchor_visual) { _anchor_visual->release(); _anchor_visual = nullptr; }
}

void SolidAnchoredSpringForceGenerator::setupVisual(PxShape* shape)
{
    if (_anchor_visual) _anchor_visual->release();
    PxTransform* t = new PxTransform(*_anchor_pos);
    _anchor_visual = new RenderItem(shape, t, { 0.8f, 0.8f, 0.1f, 1.0f });
}

void SolidAnchoredSpringForceGenerator::updateForce(RigidBody* solid, float t)
{
    // Obtenemos la posición del sólido
    Vector3 solidPos = solid->getActor()->getGlobalPose().p;
    Vector3 d = solidPos - (*_anchor_pos);
    float l = d.magnitude();

    if (l > 1e-6) {
        float displacement = l - _resting_length;
        Vector3 force = d.getNormalized() * (-_k * displacement);
        solid->addForce(force);
    }
}