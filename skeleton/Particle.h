#pragma once
#include <vector>
#include "core.hpp"
#include "RenderUtils.hpp"

class Particle
{
public:
    Particle(Vector3 Pos, Vector3 Vel, Vector3 Acel = Vector3(0, 0, 0), double dmp = 0.8, double lifetime = 10.0);
	Particle(const Particle& other);
	~Particle();

	Particle& operator=(const Particle& other);

    virtual void integrate(double t);

	void setPosition(const Vector3& pos);
	void setVelocity(const Vector3& vel);
	void setLifetime(double life);

	void setColor(const Vector4& color);

    Vector3 getPos();

	virtual void setupVisual();

	void cleanup();

	virtual bool isAlive() const;
	virtual Particle* clone() const;

protected:

	Vector3 vel;
	Vector3 acel;
	double m;
	double lifetime;
	physx::PxTransform* pose; // A render item le pasaremos la direccion de este pose, para que se actualice automaticamente
	RenderItem* renderItem;
	Vector3 prePos;
	double dmp;

	Vector4 color;
};

