#include "SolidGenerator.h"

SolidGenerator::SolidGenerator(std::string name, PxPhysics* gPhysics, PxScene* gScene) :
    _name(name),
    _gPhysics(gPhysics),
    _gScene(gScene),
    _mean_pos(0.0),
    _mean_vel(0.0, 10.0, 0.0),
    _mt(std::random_device{}()),
    _uniform_dist(0.0, 1.0),
    _normal_dist(0.0, 1.0)
{
}