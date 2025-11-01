#include "ParticleForceRegistry.h"

void ParticleForceRegistry::add(Particle* particle, ForceGenerator* fg) {
    _registry.push_back({ particle, fg });
}

void ParticleForceRegistry::remove(Particle* particle, ForceGenerator* fg) {
    ParticleForceRegistration registration_to_remove = { particle, fg };
    _registry.erase(std::remove(_registry.begin(), _registry.end(), registration_to_remove), _registry.end());
}

void ParticleForceRegistry::clear() {
    _registry.clear();
}

void ParticleForceRegistry::updateForces(double t) {
    for (auto& registration : _registry) {
        registration.fg->updateForce(registration.particle, t);
    }
}