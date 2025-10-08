#pragma once
#include <PxPhysicsAPI.h>
#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

class PhysXManager {
public:
    static PhysXManager& getInstance();

    void initialize(bool interactive = false);
    void cleanup();
    void step(float deltaTime);

    physx::PxPhysics* getPhysics() const { return gPhysics; }
    physx::PxScene* getScene() const { return gScene; }
    physx::PxMaterial* getMaterial() const { return gMaterial; }

private:
    PhysXManager() = default;
    ~PhysXManager() = default;

    // Variables de PhysX
    physx::PxDefaultAllocator gAllocator;
    physx::PxDefaultErrorCallback gErrorCallback;
    physx::PxFoundation* gFoundation = nullptr;
    physx::PxPhysics* gPhysics = nullptr;
    physx::PxMaterial* gMaterial = nullptr;
    physx::PxPvd* gPvd = nullptr;
    physx::PxDefaultCpuDispatcher* gDispatcher = nullptr;
    physx::PxScene* gScene = nullptr;
    ContactReportCallback gContactReportCallback;
};