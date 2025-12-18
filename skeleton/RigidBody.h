#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#include <PxRigidDynamic.h>

using namespace physx;

enum class RigidTargetType { NONE, RED, BLUE, BONUS };

class RigidBody
{
public:
    RigidBody(
        PxPhysics* gPhysics,
        PxScene* gScene,
        const Vector3& pos = { 0,0,0 },
        const PxGeometry& geo = PxSphereGeometry(1.0f),
        PxMaterial* material = nullptr,
        float lifetime = 10.0f
    );

    virtual ~RigidBody();

    virtual void update(float t);
    virtual bool isAlive() const;

    void addForce(const Vector3& force);
    void addForceAtPoint(const Vector3& force, const Vector3& globalPos);

    PxRigidDynamic* getActor() { return _actor; }
    void setVelocity(const Vector3& vel);
    void setColor(const Vector4& color);
    void setLifetime(float life) { _lifetime = life; }

    void setType(RigidTargetType t) { _type = t; }
    RigidTargetType getType() const { return _type; }

    void markAsHit() { _is_hit = true; }
    bool isHit() const { return _is_hit; }

    void updateHitTimer(float t) { if (_is_hit) _hit_timer += t; }
    float getHitTimer() const { return _hit_timer; }

protected:
    PxRigidDynamic* _actor;
    RenderItem* _renderItem;
    PxScene* _gScene;
    float _lifetime;

    bool _is_hit = false;
    float _hit_timer = 0.0f;

    RigidTargetType _type = RigidTargetType::NONE;
};