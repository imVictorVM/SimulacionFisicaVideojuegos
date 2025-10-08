#pragma once
#include "Projectile.h"

class CannonBall : public Projectile
{
public:
    CannonBall(Vector3 Pos, Vector3 Vel);
    void setupVisual() override;
};