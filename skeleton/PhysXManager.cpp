#include "PhysXManager.h"

using namespace physx;

PhysXManager& PhysXManager::getInstance() {
    static PhysXManager instance;
    return instance;
}

void PhysXManager::initialize(bool interactive) {
    PX_UNUSED(interactive);

    gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

    gPvd = PxCreatePvd(*gFoundation);
    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
    gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

    gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);
    gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

    // Configuración de la escena física
    PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
    gDispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = gDispatcher;
    sceneDesc.filterShader = contactReportFilterShader;
    sceneDesc.simulationEventCallback = &gContactReportCallback;
    gScene = gPhysics->createScene(sceneDesc);
}

void PhysXManager::cleanup() {
    if (gScene) {
        gScene->release();
        gScene = nullptr;
    }
    if (gDispatcher) {
        gDispatcher->release();
        gDispatcher = nullptr;
    }
    if (gPhysics) {
        gPhysics->release();
        gPhysics = nullptr;
    }
    if (gPvd) {
        PxPvdTransport* transport = gPvd->getTransport();
        gPvd->release();
        if (transport) transport->release();
    }
    if (gFoundation) {
        gFoundation->release();
        gFoundation = nullptr;
    }
}

void PhysXManager::step(float deltaTime) {
    if (gScene) {
        gScene->simulate(deltaTime);
        gScene->fetchResults(true);
    }
}