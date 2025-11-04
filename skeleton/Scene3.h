#pragma once
#include "Scene.h"
#include "core.hpp"
#include <vector>
#include <string>

class Projectile;
class ParticleForceRegistry;
class GravityForceGenerator;
class WindForceGenerator;
class WhirlwindForceGenerator;

class Scene3 : public Scene {
public:
    Scene3();
    virtual ~Scene3();

    void initialize() override;
    void update(double t) override;
    void cleanup() override;
    void handleKeyPress(unsigned char key) override;
    std::string getDescription() const override { return "Escena 3: Viento"; }

private:
    void createProjectile(int type);

    struct WhirlParticle {
        RenderItem* item;
        float angle;
    };

    void createWindZoneVisual(const Vector3& min, const Vector3& max, const Vector4& color, bool isWhirlwind = false);

    std::vector<Projectile*> _projectiles;

    ParticleForceRegistry* _force_registry;
    GravityForceGenerator* _gravity_generator;
    WindForceGenerator* _wind_generator;
    WhirlwindForceGenerator* _whirlwind_generator;

    RenderItem* _wind_area_visual;
    RenderItem* _whirlwind_area_visual;


};