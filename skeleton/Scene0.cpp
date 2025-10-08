#include "Scene0.h"
#include <iostream>
#include "AxisRenderer.h"

using namespace physx;

void Scene0::initialize() {
    std::cout << "Inicializando " << getDescription() << std::endl;

    axisRenderer = new AxisRenderer(physXManager.getPhysics(),
        physXManager.getMaterial());
    axisRenderer->createAxes(3.0f, 1.0f);
}

void Scene0::handleKeyPress(unsigned char key) {

}