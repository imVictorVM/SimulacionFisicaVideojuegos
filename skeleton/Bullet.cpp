#include "Bullet.h"
using namespace physx;

Bullet::Bullet(Vector3 Pos, Vector3 Vel)
    : Projectile(Pos, Vel,
        0.225,      // Masa de simulación calculada
        0.999,      // Damping
        8.0)        // Tiempo de vida
{
}

void Bullet::setupVisual()
{
    if (renderItem == nullptr) {
        PxShape* shShape = CreateShape(PxSphereGeometry(0.2f));
        // Se usará el color por defecto de la partícula (blanco)
        // o el que se le asigne después.
        renderItem = new RenderItem(shShape, pose, color);
    }
}