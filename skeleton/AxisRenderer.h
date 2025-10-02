#pragma once
#include <vector>
#include "myVector3D.h"
#include "core.hpp"
#include "RenderUtils.hpp"

class AxisRenderer
{
public:
    AxisRenderer(physx::PxPhysics* physics, physx::PxMaterial* material);
    ~AxisRenderer();

    void createAxes(float axisLength = 2.0f, float sphereRadius = 0.2f);
    void cleanup();

private:
    physx::PxPhysics* gPhysics;
    physx::PxMaterial* gMaterial;
    std::vector<RenderItem*> axisItems;

    void createAxis(const myVector3D& position, const Vector4& color, float sphereRadius);
};