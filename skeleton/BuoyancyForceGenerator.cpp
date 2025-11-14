#include "BuoyancyForceGenerator.h"
#include <algorithm>
#include <cmath>

BuoyancyForceGenerator::BuoyancyForceGenerator(float water_height, float liquid_density) :
    _water_height(water_height),
    _liquid_density(liquid_density)
{
}

void BuoyancyForceGenerator::updateForce(Particle* particle, double t)
{
    float h = particle->getPos().y;
    float h0 = _water_height;


    float object_height = particle->getRadius() * 2.0f;

    float object_volume = std::pow(object_height, 3);

    float immersed = 0.0f;

    if (h - h0 > object_height * 0.5f) {
        //1 Totalmente fuera del agua
        immersed = 0.0f;
    }
    else if (h0 - h > object_height * 0.5f) {
        //2 Totalmente sumergido
        immersed = 1.0f;
    }
    else {
        //3 Parcialmente sumergido
        immersed = (h0 - h) / object_height + 0.5f;
    }

    //Aplicar fuerza de flotación
    if (immersed > 0.0f)
    {
        float f_y = _liquid_density * object_volume * immersed * 9.8f;
        particle->addForce(Vector3(0.0f, f_y, 0.0f));
    }
}