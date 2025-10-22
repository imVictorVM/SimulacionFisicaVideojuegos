#include "CannonBall.h"
using namespace physx;

CannonBall::CannonBall(Vector3 Pos, Vector3 Vel)
    : Projectile(Pos, Vel,
        8.0, 400.0,        //masa real (kg) y velocidad real (m/s)
        0.998, 15.0)       //damping y tiempo de vida
{
}

void CannonBall::setupVisual()
{
    if (renderItem == nullptr) {
        PxShape* shShape = CreateShape(PxSphereGeometry(0.7f));
        renderItem = new RenderItem(shShape, pose, Vector4(0.2, 0.2, 0.2, 1));
    }
}