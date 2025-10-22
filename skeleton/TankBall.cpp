#include "TankBall.h"
using namespace physx;

TankBall::TankBall(Vector3 Pos, Vector3 Vel)
    : Projectile(Pos, Vel, 
        10.0, 1500.0,    //masa real (kg) y velocidad real (m/s)
        0.998, 10.0)     //damping y tiempo de vida
{
    
}

void TankBall::setupVisual()
{
    if (renderItem == nullptr) {
        PxShape* shShape = CreateShape(PxSphereGeometry(0.4f));
        renderItem = new RenderItem(shShape, pose, Vector4(0.8, 0.6, 0.2, 1));
    }
}