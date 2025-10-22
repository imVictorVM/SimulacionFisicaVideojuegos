#pragma once
#include "Projectile.h"

class LaserPistol : public Projectile
{
public:
    LaserPistol(Vector3 Pos, Vector3 Vel);
    void setupVisual() override;
};