#include "Scene0.h"
#include <iostream>
#include "AxisRenderer.h"
#include "RenderUtils.hpp"
using namespace physx;

extern PxPhysics* gPhysics;
extern PxMaterial* gMaterial;

void Scene0::initialize() {
    std::cout << "Inicializando " << getDescription() << std::endl;

    axisRenderer = new AxisRenderer(gPhysics, gMaterial);
    axisRenderer->createAxes(3.0f, 1.0f);
    
}

void Scene0::cleanup()
{
    axisRenderer->cleanup();
}

void Scene0::handleKeyPress(unsigned char key) {

}