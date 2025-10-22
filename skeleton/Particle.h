#pragma once
#include <vector>
#include "core.hpp"
#include "RenderUtils.hpp"

class Particle
{
public:
    Particle(Vector3 Pos, Vector3 Vel, Vector3 Acel = Vector3(0, 0, 0), double dmp = 0.8);
    ~Particle();

    virtual void integrate(double t);
    Vector3 getPos();

	virtual void setupVisual();

	void cleanup();

protected:

	Vector3 vel;
	Vector3 acel;
	double m;
	double tVida;
	physx::PxTransform* pose; // A render item le pasaremos la direccion de este pose, para que se actualice automaticamente
	RenderItem* renderItem;
	Vector3 prePos;
	double dmp;
};

