#pragma once
#include "Scene.h"
#include "ParticleSystem.h"
#include "GaussianGenerator.h"
#include "UniformGenerator.h" 
#include <vector>

class Scene2 : public Scene {
public:
    Scene2();
    virtual ~Scene2();

    void initialize() override;
    void update(double t) override;
    void cleanup() override;
    void handleKeyPress(unsigned char key) override;
    std::string getDescription() const override { return "Escena 2: Sistemas de Particulas"; }

private:
    ParticleSystem* _particleSystem;

    GaussianGenerator* fountainGenerator;
    UniformGenerator* smokeGenerator;
    GaussianGenerator* sparksGenerator;

    physx::PxVec3 original_cam_eye;
    physx::PxVec3 original_cam_dir;
};