#include "Bullet.h"
using namespace physx;

Bullet::Bullet(Vector3 Pos, Vector3 Vel)
    : Projectile(Pos, Vel, Vector3(0, -0.575, 0), 0.999, 0.17, 8.0) 
{
}

void Bullet::setupVisual()
{
    if (renderItem == nullptr) {
        PxShape* shShape = CreateShape(PxSphereGeometry(0.2f));
        renderItem = new RenderItem(shShape, pose, Vector4(1, 1, 1, 1));
    }
}