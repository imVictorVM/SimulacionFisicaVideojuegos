#include "RigidBody.h"
#include <iostream>

using namespace physx;

RigidBody::RigidBody(PxPhysics* gPhysics, PxScene* gScene, const Vector3& pos, const PxGeometry& geo, PxMaterial* material, float lifetime) :
    _gScene(gScene),
    _lifetime(lifetime)
{
    //1 Crear el actor dinámico
    _actor = gPhysics->createRigidDynamic(PxTransform(pos));

        //2 Crear la forma y el material
        PxShape* shape = CreateShape(geo, material);
        _actor->attachShape(*shape);

        //3 Establecer masa e inercia
        PxRigidBodyExt::updateMassAndInertia(*_actor, 1.0f);

        //4 Añadir a la escena
        _gScene->addActor(*_actor);

        //5 Crear el RenderItem
        _renderItem = new RenderItem(shape, _actor, { 1.0f, 1.0f, 1.0f, 1.0f });
}

RigidBody::~RigidBody()
{
    //Quitar el actor de la escena
    if (_gScene && _actor) {
        _gScene->removeActor(*_actor);
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
    //Aplica la fuerza en el centro de masas
    _actor->addForce(force, PxForceMode::eFORCE);
}

void RigidBody::addForceAtPoint(const Vector3& force, const Vector3& globalPos)
{
    //Aplica la fuerza en un punto específico
    PxRigidBodyExt::addForceAtPos(*_actor, force, globalPos, PxForceMode::eFORCE);
}

void RigidBody::setVelocity(const Vector3& vel)
{
    _actor->setLinearVelocity(vel);
}

void RigidBody::setColor(const Vector4& color)
{
    if (_renderItem) {
        _renderItem->color = color;
    }
}