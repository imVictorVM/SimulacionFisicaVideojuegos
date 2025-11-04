#include "WhirlwindForceGenerator.h"

// El constructor ahora llama al constructor de WindForceGenerator.
// Le pasamos valores nulos (Vector3(0) y k1=0) porque solo queremos
// heredar su lógica de área, no su fuerza de viento.
WhirlwindForceGenerator::WhirlwindForceGenerator(float k, const Vector3& center, const Vector3& direction) :
    WindForceGenerator(Vector3(0.0f), 0.0f, 0.0f), // Llama al constructor padre
    _K(k),
    _center(center),
    _direction(direction.getNormalized())
{
}

void WhirlwindForceGenerator::updateForce(Particle* particle, double t)
{
    if (particle->getInverseMass() == 0.0f) return;

    // 1. REUTILIZAMOS la lógica del padre para comprobar el área
    if (!isParticleInArea(particle)) return;

    // 2. Aplicamos NUESTRA lógica de fuerza (torbellino)
    Vector3 particle_to_center = particle->getPos() - _center;
    float r = particle_to_center.magnitude();
    if (r < 1e-6f) return; // evitar singularidad en el centro

    Vector3 tangential = _direction.cross(particle_to_center.getNormalized());
    Vector3 force = _K * r * tangential; // fuerza crece con la distancia

    float maxForce = 500.0f;
    if (force.magnitude() > maxForce)
        force = force.getNormalized() * maxForce;

    Vector3 radial = -particle_to_center.getNormalized();
    force += radial * (_K * 0.2f); // pequeña succión hacia el centro

    particle->addForce(force);
}