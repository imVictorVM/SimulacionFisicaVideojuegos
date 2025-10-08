#pragma once
#include "Projectile.h"

class Bullet : public Projectile
{
public:
    Bullet(Vector3 Pos, Vector3 Vel);
    void setupVisual() override;
};