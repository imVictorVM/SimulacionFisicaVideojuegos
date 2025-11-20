#include "GaussianSolidGenerator.h"

GaussianSolidGenerator::GaussianSolidGenerator(std::string name, PxPhysics* gPhysics, PxScene* gScene) :
    SolidGenerator(name, gPhysics, gScene),
    _vel_deviation(1.0, 1.0, 1.0),
    _half_extents(1.0, 1.0, 1.0),
    _material(nullptr)
{
}

std::list<RigidBody*> GaussianSolidGenerator::generateSolids()
{
    std::list<RigidBody*> new_solids;
    if (!_is_active) return new_solids;

    for (int i = 0; i < _num_per_generation; ++i)
    {
        //1 Crear el sólido
        RigidBody* rb = new RigidBody(_gPhysics, _gScene, _mean_pos, PxBoxGeometry(_half_extents), _material, 15.0f);

        //2 Calcular nueva velocidad
        Vector3 new_vel = _mean_vel;
        new_vel.x += _normal_dist(_mt) * _vel_deviation.x;
        new_vel.y += _normal_dist(_mt) * _vel_deviation.y;
        new_vel.z += _normal_dist(_mt) * _vel_deviation.z;
        rb->setVelocity(new_vel);

        //3 Variar la inercia
        //Cambia la densidad aleatoriamente entre 0.5 y 2.5
        float density = 0.5f + (float)_uniform_dist(_mt) * 2.0f;
        PxRigidBodyExt::updateMassAndInertia(*rb->getActor(), density);

        //4 Asignar color aleatorio
        rb->setColor({
            (float)_uniform_dist(_mt),
            (float)_uniform_dist(_mt),
            (float)_uniform_dist(_mt),
            1.0f
            });

        new_solids.push_back(rb);
    }
    return new_solids;
}