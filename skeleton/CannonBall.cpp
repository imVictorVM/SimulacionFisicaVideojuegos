#include "CannonBall.h"
using namespace physx;
CannonBall::CannonBall(Vector3 Pos, Vector3 Vel)
    : Projectile(Pos, Vel,
        800.0,      // Masa de simulación calculada
        0.998,      // Damping
        15.0)       // Tiempo de vida
{
}

void CannonBall::setupVisual()
{
    if (renderItem == nullptr) {
        PxShape* shShape = CreateShape(PxSphereGeometry(0.7f));
        setColor({ 0.2, 0.2, 0.2, 1 }); // Asignamos el color a la partícula
        renderItem = new RenderItem(shShape, pose, color);

    }
}