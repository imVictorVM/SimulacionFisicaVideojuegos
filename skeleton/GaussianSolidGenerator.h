#pragma once
#include "SolidGenerator.h"


class GaussianSolidGenerator : public SolidGenerator {
public:
    GaussianSolidGenerator(std::string name, PxPhysics* gPhysics, PxScene* gScene);
    virtual ~GaussianSolidGenerator() {}

    virtual std::list<RigidBody*> generateSolids() override;

    void setVelDeviation(const Vector3& deviation) { _vel_deviation = deviation; }
    void setBoxHalfExtents(const Vector3& halfExtents) { _half_extents = halfExtents; }
    void setMaterial(PxMaterial* mat) { _material = mat; }

protected:
    Vector3 _vel_deviation;
    Vector3 _half_extents;
    PxMaterial* _material;
};