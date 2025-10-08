#include "CannonBall.h"
using namespace physx;

CannonBall::CannonBall(Vector3 Pos, Vector3 Vel)
    : Projectile(Pos, Vel, Vector3(0, -0.25, 0), 0.998, 78.125, 15.0)
{
}

void CannonBall::setupVisual()
{
    if (renderItem == nullptr) {
        PxShape* shShape = CreateShape(PxSphereGeometry(0.7f));
        renderItem = new RenderItem(shShape, pose, Vector4(0.2, 0.2, 0.2, 1));
    }
}