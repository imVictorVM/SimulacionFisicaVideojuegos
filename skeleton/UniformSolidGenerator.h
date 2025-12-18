#pragma once
#include "SolidGenerator.h"
#include "RigidBody.h" 

class UniformSolidGenerator : public SolidGenerator {
public:
    UniformSolidGenerator(std::string name, PxPhysics* gPhysics, PxScene* gScene);
    virtual ~UniformSolidGenerator() {}

    virtual std::list<RigidBody*> generateSolids() override;

    void setPosRange(const Vector3& min_pos, const Vector3& max_pos);
    void setVelRange(const Vector3& min_vel, const Vector3& max_vel);
    void setMaterial(PxMaterial* mat) { _material = mat; }

    void setGenerationColor(const Vector4& color) { _gen_color = color; }

    void setGenerationType(RigidTargetType type) { _gen_type = type; }

protected:
    Vector3 _min_pos, _max_pos;
    Vector3 _min_vel, _max_vel;
    PxMaterial* _material;
    Vector4 _gen_color;

    RigidTargetType _gen_type = RigidTargetType::NONE;
};