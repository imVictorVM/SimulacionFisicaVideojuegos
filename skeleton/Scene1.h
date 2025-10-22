#pragma once
#include "Scene.h"
#include "Bullet.h"
#include "CannonBall.h"
#include "TankBall.h"
#include "LaserPistol.h"

class Scene1 : public Scene {
public:
    void initialize() override;
    void cleanup() override;
    void handleKeyPress(unsigned char key) override;
    std::string getDescription() const override { return "Escena 1"; }

private:
    void createProjectile(int type);
};