#include "ParticleForceRegistry.h"

void ParticleForceRegistry::add(Particle* particle, ForceGenerator* fg) {
    _registry.push_back({ particle, fg });
}

void ParticleForceRegistry::remove(Particle* particle, ForceGenerator* fg) {
}

void ParticleForceRegistry::clear() {
    _registry.clear();
}

void ParticleForceRegistry::updateForces(double t) {
    for (auto& registration : _registry) {
        registration.fg->updateForce(registration.particle, t);
    }
}