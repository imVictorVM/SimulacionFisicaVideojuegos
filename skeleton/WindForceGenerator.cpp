#include "WindForceGenerator.h"
#include <algorithm>
#include <cmath>

WindForceGenerator::WindForceGenerator(const Vector3& wind_vel, float k1, float k2) :
    _wind_velocity(wind_vel),
    _k1(k1),
    _k2(k2),
    _area_active(false)
{
}

void WindForceGenerator::setWindArea(const Vector3& corner1, const Vector3& corner2)
{
    _area_corner1.x = (std::min)(corner1.x, corner2.x);
    _area_corner1.y = (std::min)(corner1.y, corner2.y);
    _area_corner1.z = (std::min)(corner1.z, corner2.z);

    _area_corner2.x = (std::max)(corner1.x, corner2.x);
    _area_corner2.y = (std::max)(corner1.y, corner2.y);
    _area_corner2.z = (std::max)(corner1.z, corner2.z);

    _area_active = true;
}

bool WindForceGenerator::isParticleInArea(Particle* particle)
{
    // Si el área no está activa, actúa en todas partes
    if (!_area_active) return true;

    Vector3 pos = particle->getPos();
    if (pos.x < _area_corner1.x || pos.x > _area_corner2.x ||
        pos.y < _area_corner1.y || pos.y > _area_corner2.y ||
        pos.z < _area_corner1.z || pos.z > _area_corner2.z)
    {
        return false; // Está fuera
    }
    return true; // Está dentro
}

void WindForceGenerator::updateForce(Particle* particle, double t)
{
    if (particle->getInverseMass() == 0.0f) {
        return;
    }

    if (!isParticleInArea(particle)) return;

    Vector3 windVel = _wind_velocity + Vector3(
        sin(t * 0.5) * 2.0f,
        cos(t * 0.7) * 1.0f,
        0.0f
    );
    Vector3 relative_velocity = windVel - particle->getVelocity();
    Vector3 wind_force = relative_velocity * _k1;

    if (_k2 > 0) {
        float drag_magnitude = relative_velocity.magnitude();
        if (drag_magnitude > 1e-6) { // Evitar división por cero
            Vector3 drag_force = relative_velocity.getNormalized() * (drag_magnitude * drag_magnitude * _k2);
            wind_force += drag_force;
        }
    }



    particle->addForce(wind_force);
}