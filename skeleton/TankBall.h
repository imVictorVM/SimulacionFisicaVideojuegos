#pragma once
#include "Projectile.h"

class TankBall : public Projectile
{
public:
    TankBall(Vector3 Pos, Vector3 Vel);
    void setupVisual() override;
};