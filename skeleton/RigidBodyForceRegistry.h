#pragma once
#include "RigidBody.h"
#include "SolidForceGenerator.h"
#include <vector>
#include <algorithm>

class RigidBodyForceRegistry {
protected:
    struct Registration {
        RigidBody* solid;
        SolidForceGenerator* fg;

        bool operator==(const Registration& other) const {
            return solid == other.solid && fg == other.fg;
        }
    };

    std::vector<Registration> _registry;

public:
    void add(RigidBody* solid, SolidForceGenerator* fg) {
        _registry.push_back({ solid, fg });
    }

    void remove(RigidBody* solid, SolidForceGenerator* fg) {
        Registration reg = { solid, fg };
        _registry.erase(std::remove(_registry.begin(), _registry.end(), reg), _registry.end());
    }

    void clear() {
        _registry.clear();
    }

    void updateForces(float t) {
        for (auto& reg : _registry) {
            reg.fg->updateForce(reg.solid, t);
        }
    }
};