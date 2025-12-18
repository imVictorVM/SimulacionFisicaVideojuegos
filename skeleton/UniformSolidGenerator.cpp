#include "UniformSolidGenerator.h"
#include <extensions/PxRigidBodyExt.h> 

UniformSolidGenerator::UniformSolidGenerator(std::string name, PxPhysics* gPhysics, PxScene* gScene) :
    SolidGenerator(name, gPhysics, gScene),
    _min_pos(-1, -1, -1), _max_pos(1, 1, 1),
    _min_vel(0, 0, 0), _max_vel(0, 0, 0),
    _material(nullptr),
    _gen_color(1, 1, 1, 1),
    _gen_type(RigidTargetType::NONE)
{
}

void UniformSolidGenerator::setPosRange(const Vector3& min, const Vector3& max) {
    _min_pos = min; _max_pos = max;
}

void UniformSolidGenerator::setVelRange(const Vector3& min, const Vector3& max) {
    _min_vel = min; _max_vel = max;
}

std::list<RigidBody*> UniformSolidGenerator::generateSolids()
{
    std::list<RigidBody*> new_solids;
    if (!_is_active) return new_solids;

    for (int i = 0; i < _num_per_generation; ++i) {
        Vector3 pos;
        pos.x = _min_pos.x + _uniform_dist(_mt) * (_max_pos.x - _min_pos.x);
        pos.y = _min_pos.y + _uniform_dist(_mt) * (_max_pos.y - _min_pos.y);
        pos.z = _min_pos.z + _uniform_dist(_mt) * (_max_pos.z - _min_pos.z);

        Vector3 vel;
        vel.x = _min_vel.x + _uniform_dist(_mt) * (_max_vel.x - _min_vel.x);
        vel.y = _min_vel.y + _uniform_dist(_mt) * (_max_vel.y - _min_vel.y);
        vel.z = _min_vel.z + _uniform_dist(_mt) * (_max_vel.z - _min_vel.z);

        RigidBody* rb = new RigidBody(_gPhysics, _gScene, pos, PxBoxGeometry(1.5f, 1.5f, 1.5f), _material, 20.0f);
        rb->setVelocity(vel);
        rb->setColor(_gen_color);
        rb->setType(_gen_type);

        rb->getActor()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

        // Densidad 50 vs Agua 1000 -> Flota 95% fuera del agua
        float density = 50.0f;
        PxRigidBodyExt::updateMassAndInertia(*rb->getActor(), density);

        new_solids.push_back(rb);
    }
    return new_solids;
}