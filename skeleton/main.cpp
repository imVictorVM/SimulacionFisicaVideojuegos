#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>

#include "myVector3D.h"
#include "AxisRenderer.h"

#include "Particle.h"

std::string display_text = "This is a test";


using namespace physx;
using namespace std;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;

RenderItem* s = nullptr;

//EJES DE COORDENADAS
AxisRenderer* axisRenderer = nullptr;

//vector para poder guardar partículas y no tener problemas con los deregister
vector<Particle*> prac1Particles;

void createParticle(Vector3 pos, Vector3 vel, Vector3 acel = Vector3(0,0,0), double dmp = 0) {
	Particle* part = new Particle(pos, vel, acel, dmp);
	prac1Particles.push_back(part);
}


// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	 /*
	PxShape* shShape = CreateShape(PxSphereGeometry(10),gMaterial);
	PxTransform* spTransform = new PxTransform(PxVec3(0,0,0));
	s = new RenderItem(shShape, spTransform, Vector4(0, 0, 1, 1));
	RegisterRenderItem(s);
	*/

	axisRenderer = new AxisRenderer(gPhysics, gMaterial);
	axisRenderer->createAxes(3.0f, 1.0f);

	
	
	
	}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	for (auto p : prac1Particles) {
		p->integrate(t);
		Vector3 pos = p->getPos();
		std::cout << "Pos: (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;

	}

	gScene->simulate(t);
	gScene->fetchResults(true);
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	//COSAS QUE HE CREADO
	if (axisRenderer != nullptr) {
		delete axisRenderer;
		axisRenderer = nullptr;
	}

	for (auto p : prac1Particles) {
		delete p;
	}
	prac1Particles.clear();

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	gFoundation->release();

	//DeregisterRenderItem(s);
	}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch(toupper(key))
	{
	case 'B':
	{
		Vector3 posicionInicial(0, 0, 0);      
		Vector3 velocidadInicial(0, 12, 0);      
		Vector3 aceleracion(0, -5, 0);          
		double dmp = 0.95;                  

		createParticle(posicionInicial, velocidadInicial, aceleracion, dmp);
		break;
	}
		//case ' ':	break;
	case ' ':
	{
		break;
	}
	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char*const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}