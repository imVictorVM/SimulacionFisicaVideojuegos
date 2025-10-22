#pragma once
#include <vector>
#include "Particle.h"
#include "Projectile.h"


class Scene {
public:
    Scene() {};
    virtual ~Scene() { cleanup(); }

    virtual void initialize() = 0;
    virtual void cleanup();
    virtual void update(double t);
    virtual void handleKeyPress(unsigned char key) = 0;
    virtual std::string getDescription() const = 0;


    std::vector<Projectile*>& getProjectiles() { return projectiles; }

protected:
    std::vector<Projectile*> projectiles;
};