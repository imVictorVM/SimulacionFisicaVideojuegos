#include "AxisRenderer.h"
#include <iostream>

using namespace physx;

AxisRenderer::AxisRenderer(PxPhysics* physics, PxMaterial* material)
    : gPhysics(physics), gMaterial(material)
{
}

AxisRenderer::~AxisRenderer()
{
    cleanup();
}

void AxisRenderer::createAxes(float axisLength, float sphereRadius)
{
    // Limpiar ejes existentes si los hay
    cleanup();

    // Definir los ejes usando myVector3D
    myVector3D ejeX(axisLength, 0.0, 0.0);  // Eje X - Rojo
    myVector3D ejeY(0.0, axisLength, 0.0);  // Eje Y - Verde  
    myVector3D ejeZ(0.0, 0.0, axisLength);  // Eje Z - Azul
    myVector3D origen(0.0, 0.0, 0.0);       // Origen - Blanco

    // Crear las esferas para cada eje
    createAxis(ejeX, Vector4(1.0f, 0.0f, 0.0f, 1.0f), sphereRadius); // Rojo
    createAxis(ejeY, Vector4(0.0f, 1.0f, 0.0f, 1.0f), sphereRadius); // Verde
    createAxis(ejeZ, Vector4(0.0f, 0.0f, 1.0f, 1.0f), sphereRadius); // Azul
    createAxis(origen, Vector4(1.0f, 1.0f, 1.0f, 1.0f), sphereRadius * 0.5f); // Blanco (más pequeño)

    std::cout << "Ejes de coordenadas creados exitosamente!" << std::endl;
    std::cout << " - Eje X (Rojo): (" << ejeX.getX() << ", " << ejeX.getY() << ", " << ejeX.getZ() << ")" << std::endl;
    std::cout << " - Eje Y (Verde): (" << ejeY.getX() << ", " << ejeY.getY() << ", " << ejeY.getZ() << ")" << std::endl;
    std::cout << " - Eje Z (Azul): (" << ejeZ.getX() << ", " << ejeZ.getY() << ", " << ejeZ.getZ() << ")" << std::endl;
}

void AxisRenderer::createAxis(const myVector3D& position, const Vector4& color, float sphereRadius)
{
    // Crear forma física
    PxShape* shape = CreateShape(PxSphereGeometry(sphereRadius), gMaterial);

    // Crear transform con la posición
    PxTransform* transform = new PxTransform(PxVec3((float)position.getX(), (float)position.getY(), (float)position.getZ()));

    // Crear RenderItem
    RenderItem* item = new RenderItem(shape, transform, color);
    axisItems.push_back(item);
}

void AxisRenderer::cleanup()
{
    for (auto item : axisItems) {
        DeregisterRenderItem(item);
        delete item;
    }
    axisItems.clear();
}