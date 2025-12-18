#include "SolidBuoyancyForceGenerator.h"
#include <algorithm>
#include <cmath>

SolidBuoyancyForceGenerator::SolidBuoyancyForceGenerator(float water_height, float liquid_density) :
    _water_height(water_height), _liquid_density(liquid_density)
{
}

void SolidBuoyancyForceGenerator::updateForce(RigidBody* solid, float t)
{
    Vector3 pos = solid->getActor()->getGlobalPose().p;
    float h = pos.y;
    float h0 = _water_height;

    float object_height = 1.5f;
    float object_volume = std::pow(object_height, 3);

    float immersed = 0.0f;
    if (h - h0 > object_height * 0.5f) immersed = 0.0f;
    else if (h0 - h > object_height * 0.5f) immersed = 1.0f;
    else immersed = (h0 - h) / object_height + 0.5f;

    if (immersed > 0.0f) {
        float f_y = _liquid_density * object_volume * immersed * 9.8f;
        // Aplicamos fuerza en el centro
        solid->addForce(Vector3(0.0f, f_y, 0.0f));

        Vector3 vel = solid->getActor()->getLinearVelocity();
        solid->addForce(-vel * 0.5f * immersed * _liquid_density);
    }
}