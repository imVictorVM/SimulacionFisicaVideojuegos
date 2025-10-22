#include "Bullet.h"
using namespace physx;

Bullet::Bullet(Vector3 Pos, Vector3 Vel)
    : Projectile(Pos, Vel,
        0.002, 850.0,      //masa real (kg) y velocidad real (m/s)
        0.999, 8.0)        //damping y tiempo de vida
{
}

void Bullet::setupVisual()
{
    if (renderItem == nullptr) {
        PxShape* shShape = CreateShape(PxSphereGeometry(0.2f));
        renderItem = new RenderItem(shShape, pose, Vector4(1, 1, 1, 1));
    }
}