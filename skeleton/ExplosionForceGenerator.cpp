#include "ExplosionForceGenerator.h"
#include <cmath>

ExplosionForceGenerator::ExplosionForceGenerator(double k, double t_constant, double radius, const Vector3& center) :
    _K(k),
    _T(t_constant),
    _R(radius),
    _center(center),
    _is_active(false),
    _time_elapsed(0.0)
{
}

void ExplosionForceGenerator::trigger()
{
    _is_active = true;
    _time_elapsed = 0.0;
}

void ExplosionForceGenerator::setCenter(const Vector3& center)
{
    _center = center;
}

void ExplosionForceGenerator::updateForce(Particle* particle, double t)
{
    //1 Si la explosión no está activa, no hacer nada.
    if (!_is_active) return;

    //2 Actualizar el tiempo transcurrido
    _time_elapsed += t;

    //3 Si la explosión ya se ha desvanecido (pasados 4*T), desactivarla.
    if (_time_elapsed > (4 * _T)) {
        _is_active = false;
        return;
    }


    //Vector desde el centro de la explosión a la partícula
    Vector3 direction = particle->getPos() - _center;

    double r2 = direction.magnitudeSquared();
    double r = std::sqrt(r2);

    //4 Comprobar si la partícula está dentro del radio y no es el centro exacto
    if (r < _R && r > 1e-6) // r > 1e-6 para evitar división por cero
    {
        //5 Calcular los componentes de la fórmula

        //K / r^2
        double force_magnitude = _K / r2;

        //e^(-t / T)
        double decay = std::exp(-_time_elapsed / _T);

        //6 Calcular la fuerza final
        //(K / r^2) * e^(-t/T) * (dirección normalizada)
        Vector3 force = direction.getNormalized() * force_magnitude * decay;

        //7 Aplicar la fuerza a la partícula
        particle->addForce(force);
    }
}