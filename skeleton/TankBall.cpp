#include "TankBall.h"
using namespace physx;

TankBall::TankBall(Vector3 Pos, Vector3 Vel)
    : Projectile(Pos, Vel,
        1000.0,     // Masa de simulación calculada
        0.998,      // Damping
        10.0)       // Tiempo de vida
{
}

void TankBall::setupVisual()
{
    if (renderItem == nullptr) {
        PxShape* shShape = CreateShape(PxSphereGeometry(0.4f));
        setColor({ 0.8, 0.6, 0.2, 1 }); // Asignamos el color
        renderItem = new RenderItem(shShape, pose, color);
    }
}