#pragma once
#include "Scene.h"
#include "RigidBody.h"
#include "RigidBodyForceRegistry.h"
#include "GaussianSolidGenerator.h"
#include "ExplosionSolidForceGenerator.h"
#include <vector>
#include <list>

class Scene7 : public Scene {
public:
    Scene7();
    virtual ~Scene7();

    void initialize() override;
    void update(double t) override;
    void cleanup() override;
    void handleKeyPress(unsigned char key) override;
    std::string getDescription() const override { return "Escena 7: Solidos Rigidos"; }

private:
    void createStaticFloor();
    void createStaticObstacles();
    void spawnManualInertiaBox();

    void shoot();

    RigidBodyForceRegistry* _rb_registry;
    GaussianSolidGenerator* _solid_generator;
    ExplosionSolidForceGenerator* _explosion;

    std::list<RigidBody*> _solids;
    std::vector<RenderItem*> _static_visuals; //Para limpiar los estáticos
    std::vector<PxRigidStatic*> _static_actors; //Para limpiar los estáticos

    PxMaterial* _material;//Material para fricción y rebote

    //Timers para que no se llene la escena de sólidos
    double _spawn_timer;
    double _spawn_interval;
};