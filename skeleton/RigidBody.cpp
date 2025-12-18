#include "RigidBody.h"
#include <iostream>

using namespace physx;

RigidBody::RigidBody(PxPhysics* gPhysics, PxScene* gScene, const Vector3& pos, const PxGeometry& geo, PxMaterial* material, float lifetime) :
    _gScene(gScene),
    _lifetime(lifetime),
    _is_hit(false),
    _hit_timer(0.0f),
    _type(RigidTargetType::NONE)
{
    _actor = gPhysics->createRigidDynamic(PxTransform(pos));

    PxShape* shape = CreateShape(geo, material);
    _actor->attachShape(*shape);

    PxRigidBodyExt::updateMassAndInertia(*_actor, 1.0f);

    _gScene->addActor(*_actor);

    _renderItem = new RenderItem(shape, _actor, { 1.0f, 1.0f, 1.0f, 1.0f });
}

RigidBody::~RigidBody()
{
    if (_gScene && _actor) {
        _gScene->removeActor(*_actor);
    }

    if (_actor) {
        _actor->release();
        _actor = nullptr;
    }

    if (_renderItem) {
        _renderItem->release();
        _renderItem = nullptr;
    }
}

void RigidBody::update(float t)
{
    _lifetime -= t;
}

bool RigidBody::isAlive() const
{
    return _lifetime > 0.0f;
}

void RigidBody::addForce(const Vector3& force)
{
    if (_actor) _actor->addForce(force, PxForceMode::eFORCE);
}

void RigidBody::addForceAtPoint(const Vector3& force, const Vector3& globalPos)
{
    if (_actor) PxRigidBodyExt::addForceAtPos(*_actor, force, globalPos, PxForceMode::eFORCE);
}

void RigidBody::setVelocity(const Vector3& vel)
{
    if (_actor) _actor->setLinearVelocity(vel);
}

void RigidBody::setColor(const Vector4& color)
{
    if (_renderItem) {
        _renderItem->color = color;
    }
}