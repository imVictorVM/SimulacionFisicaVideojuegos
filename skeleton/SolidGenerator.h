#pragma once
#include "RigidBody.h"
#include <list>
#include <random>
#include <string>
#include <memory>

class SolidGenerator {
public:
    SolidGenerator(std::string name, PxPhysics* gPhysics, PxScene* gScene);
    virtual ~SolidGenerator() {}

    virtual std::list<RigidBody*> generateSolids() = 0;

    void setMeanPosition(const Vector3& pos) { _mean_pos = pos; }
    void setMeanVelocity(const Vector3& vel) { _mean_vel = vel; }
    void setNumObjects(int num) { _num_per_generation = num; }

    void setActive(bool active) { _is_active = active; }
    void toggleActive() { _is_active = !_is_active; }

protected:
    std::string _name;
    Vector3 _mean_pos;
    Vector3 _mean_vel;
    int _num_per_generation = 1;
    bool _is_active = true;

    //Punteros necesarios para crear RigidBody
    PxPhysics* _gPhysics;
    PxScene* _gScene;

    std::mt19937 _mt;
    std::uniform_real_distribution<double> _uniform_dist;
    std::normal_distribution<double> _normal_dist;
};