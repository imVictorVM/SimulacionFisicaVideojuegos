#pragma once
#include <vector>
#include "Particle.h"
#include "Projectile.h"

/////////////////////////////
#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>


class Scene {
public:
    Scene() {}
    virtual ~Scene() { cleanup(); }


    void initPhysx(bool interactive);
    virtual void initialize() = 0;
    virtual void cleanup();
    virtual void update(double t);
    virtual void handleKeyPress(unsigned char key) = 0;

    virtual std::string getDescription() const = 0;

    // Métodos para acceder a las partículas/proyectiles
    std::vector<Particle*>& getParticles() { return particles; }
    std::vector<Projectile*>& getProjectiles() { return projectiles; }

protected:
    std::vector<Particle*> particles;
    std::vector<Projectile*> projectiles;
    ///////////////////////////////////////////////////////
    physx::PxDefaultAllocator		gAllocator;
    physx::PxDefaultErrorCallback	gErrorCallback;

    physx::PxFoundation* gFoundation = NULL;
    physx::PxPhysics* gPhysics = NULL;


    physx::PxMaterial* gMaterial = NULL;

    physx::PxPvd* gPvd = NULL;

    physx::PxDefaultCpuDispatcher* gDispatcher = NULL;
    physx::PxScene* gScene = NULL;
    ContactReportCallback gContactReportCallback;

    RenderItem* s = nullptr;


};