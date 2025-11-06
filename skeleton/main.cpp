

#include <PxPhysicsAPI.h>


#include <ctype.h>
#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>
#include <thread>
#include <chrono>

#include "SceneManager.h"



//std::string display_text = "This is a test";

std::string display_text = "";

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

//ESCENAS
SceneManager* sceneManager = nullptr;


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
	
	//CREAR SCENE MANAGER
	sceneManager = new SceneManager();
	sceneManager->initialize();
	
	}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	//UPDATE DE LAS ESCENAS DESDE SCENEMANAGER
	if (sceneManager) {
		sceneManager->update(t);
	}

	gScene->simulate(t);
	gScene->fetchResults(true);


	//IMPORTANTE PARA ELIMINAR RUIDO
	std::this_thread::sleep_for(std::chrono::microseconds(10));
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	//COSAS QUE HE CREADO
	//SCENE MANAGER
	if (sceneManager != nullptr) {
		delete sceneManager;
		sceneManager = nullptr;
	}
	std::this_thread::sleep_for(std::chrono::microseconds(10));

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	gFoundation->release();
	
	}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);
	//CAMBIOS DE ESCENA
	if (key >= '0' && key <= '9') {
		int sceneNum = key - '0';
		if (sceneManager) {
			sceneManager->switchToScene(static_cast<SceneManager::SceneType>(sceneNum));
		}
	}
	// Otras teclas se pasan a la escena actual
	else {
		if (sceneManager) {
			sceneManager->handleKeyPress(key);
		}
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char*const*)
{
#ifdef _DEBUG

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
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
	_CrtDumpMemoryLeaks();
	return 0;
}