#include "ExplosionSolidForceGenerator.h"
#include <cmath>

ExplosionSolidForceGenerator::ExplosionSolidForceGenerator(double k, double t_constant, double radius, const Vector3& center) :
    _K(k),
    _T(t_constant),
    _R(radius),
    _center(center),
    _is_active(false),
    _time_elapsed(0.0)
{
}

void ExplosionSolidForceGenerator::update(double t)
{
    if (!_is_active) return;

    //Actualizamos el tiempo global de la explosión 
    _time_elapsed += t;

    if (_time_elapsed > (0.25 * _T)) {
        _is_active = false;
    }
}

void ExplosionSolidForceGenerator::updateForce(RigidBody* solid, float t)
{
    if (!_is_active) return;

    Vector3 solidPos = solid->getActor()->getGlobalPose().p;
    Vector3 direction = solidPos - _center;
    double r2 = direction.magnitudeSquared();
    double r = std::sqrt(r2);

    if (r < _R && r > 1e-6)
    {
        double force_magnitude = _K / r2;
        double decay = std::exp(-_time_elapsed / _T);
        Vector3 force = direction.getNormalized() * force_magnitude * decay;

        solid->addForce(force);
        //Aplicamos la fuerza en la posición global del objeto,
        //que NO es su centro de masas si el objeto está rotando.
        //Esto generará torque.
        Vector3 torquePoint = solidPos + Vector3(0.5f, 0.5f, 0.5f);
        solid->addForceAtPoint(force * 0.5f, torquePoint);
    }
}