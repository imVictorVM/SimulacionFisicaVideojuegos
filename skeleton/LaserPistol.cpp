#include "LaserPistol.h"
using namespace physx;

LaserPistol::LaserPistol(Vector3 Pos, Vector3 Vel)

    : Projectile(Pos, Vel,
        0.0, 300000000.0, // Masa real (kg) y velocidad real (m/s)
        1.0, 4.0)         //damping 1.0 y tiempo de vida corto
{  
    acel = Vector3(0, 0.1, 0);
}

void LaserPistol::setupVisual()
{
    if (renderItem == nullptr) {
        PxShape* shShape = CreateShape(PxSphereGeometry(0.1f));
        renderItem = new RenderItem(shShape, pose, Vector4(1.0, 0.1, 0.1, 1));
    }
}