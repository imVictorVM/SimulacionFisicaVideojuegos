#include "AxisRenderer.h"
#include <iostream>

using namespace physx;

AxisRenderer::AxisRenderer(PxPhysics* physics, PxMaterial* material)
    : gPhysics(physics), gMaterial(material), xTr(nullptr), yTr(nullptr), zTr(nullptr), orTr(nullptr)
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
    createAxis(xTr, ejeX, Vector4(1.0f, 0.0f, 0.0f, 1.0f), sphereRadius); // Rojo
    createAxis(yTr, ejeY, Vector4(0.0f, 1.0f, 0.0f, 1.0f), sphereRadius); // Verde
    createAxis(zTr, ejeZ, Vector4(0.0f, 0.0f, 1.0f, 1.0f), sphereRadius); // Azul
    createAxis(orTr, origen, Vector4(1.0f, 1.0f, 1.0f, 1.0f), sphereRadius * 0.5f); // Blanco (más pequeño)

    std::cout << "Ejes de coordenadas creados exitosamente!" << std::endl;
    std::cout << " - Eje X (Rojo): (" << ejeX.getX() << ", " << ejeX.getY() << ", " << ejeX.getZ() << ")" << std::endl;
    std::cout << " - Eje Y (Verde): (" << ejeY.getX() << ", " << ejeY.getY() << ", " << ejeY.getZ() << ")" << std::endl;
    std::cout << " - Eje Z (Azul): (" << ejeZ.getX() << ", " << ejeZ.getY() << ", " << ejeZ.getZ() << ")" << std::endl;
}

void AxisRenderer::createAxis(physx::PxTransform* tr, const myVector3D& position, const Vector4& color, float sphereRadius)
{
    // Crear forma física
    PxShape* shape = CreateShape(PxSphereGeometry(sphereRadius), gMaterial);

    // Crear transform con la posición
    tr = new PxTransform(PxVec3((float)position.getX(), (float)position.getY(), (float)position.getZ()));

    // Crear RenderItem
    RenderItem* item = new RenderItem(shape, tr, color);
    axisItems.push_back(item);
}

void AxisRenderer::cleanup()
{
    for (auto item : axisItems) {
        DeregisterRenderItem(item);
        delete item;
    }
    axisItems.clear();
    if(xTr != nullptr) delete xTr;
    if(yTr != nullptr) delete yTr;
    if(zTr != nullptr) delete zTr;
    if(orTr != nullptr) delete orTr;
}