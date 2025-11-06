#include "LaserPistol.h"
using namespace physx;

LaserPistol::LaserPistol(Vector3 Pos, Vector3 Vel)
    : Projectile(Pos, Vel,
        0.0,        // Masa 0 (o masa infinita), no le afectan las fuerzas
        1.0,        // Damping (sin resistencia)
        4.0)        // Tiempo de vida
{
}

void LaserPistol::setupVisual()
{
    if (renderItem == nullptr) {
        PxShape* shShape = CreateShape(PxSphereGeometry(0.1f));
        //setColor({ 1.0, 0.1, 0.1, 1 }); // Asignamos el color
        renderItem = new RenderItem(shShape, pose, color);
    }
}