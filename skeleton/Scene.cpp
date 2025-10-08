#include "Scene.h"

using namespace physx;
using namespace std;

void Scene::initPhysx(bool interactive)
{
    PX_UNUSED(interactive);

    gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

    gPvd = PxCreatePvd(*gFoundation);
    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
    gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

    gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

    gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

    // For Solid Rigids +++++++++++++++++++++++++++++++++++++
    PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
    gDispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = gDispatcher;
    sceneDesc.filterShader = contactReportFilterShader;
    sceneDesc.simulationEventCallback = &gContactReportCallback;
    gScene = gPhysics->createScene(sceneDesc);
}

void Scene::cleanup() {
    for (auto p : particles) {
        delete p;
    }
    particles.clear();

    for (auto p : projectiles) {
        delete p;
    }
    projectiles.clear();
}

void Scene::update(double t) {
    for (auto p : particles) {
        p->integrate(t);
    }

    // Actualizar proyectiles y limpiar los muertos
    for (auto it = projectiles.begin(); it != projectiles.end(); ) {
        if (!(*it)->isAlive()) {
            delete* it;
            it = projectiles.erase(it);
        }
        else {
            (*it)->integrate(t);
            ++it;
        }
    }
}