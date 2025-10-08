#pragma once
#include <vector>
#include "Particle.h"
#include "Projectile.h"
#include "PhysXManager.h"  

class Scene {
public:
    Scene() : physXManager(PhysXManager::getInstance()) {}  
    virtual ~Scene() { cleanup(); }

    virtual void initialize() = 0;
    virtual void cleanup();
    virtual void update(double t);
    virtual void handleKeyPress(unsigned char key) = 0;
    virtual std::string getDescription() const = 0;

    std::vector<Particle*>& getParticles() { return particles; }
    std::vector<Projectile*>& getProjectiles() { return projectiles; }

protected:
    std::vector<Particle*> particles;
    std::vector<Projectile*> projectiles;
    PhysXManager& physXManager;  
};