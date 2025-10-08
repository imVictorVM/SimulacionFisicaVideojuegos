

#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>

#include "SceneManager.h"


std::string display_text = "This is a test";


using namespace physx;
using namespace std;

/*

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

*/

//EJES DE COORDENADAS
//AxisRenderer* axisRenderer = nullptr;

//vector para poder guardar partículas y no tener problemas con los deregister
//vector<Particle*> prac1Particles;

//SUELO
//RenderItem* ground = nullptr;
//RenderItem* target = nullptr;

//ESCENAS
SceneManager* sceneManager = nullptr;

//vector<Projectile*> prac1Projectiles;
/*
void createParticle(Vector3 pos, Vector3 vel, Vector3 acel = Vector3(0,0,0), double dmp = 0) {
	Particle* part = new Particle(pos, vel, acel, dmp);
	part->setupVisual();
	prac1Particles.push_back(part);
}

void createProjectile(int type) {

	Camera* cam = GetCamera();
	
	Vector3 startPos = cam->getEye();
	Vector3 direction = cam->getDir();

	Projectile* projectile = nullptr;

	switch (type) {
	case 1: // Bala
		projectile = new Bullet(startPos, direction * 80.0f);
		break;
	case 2: // Bola de cañón
		projectile = new CannonBall(startPos, direction * 40.0f);
		break;
	}

	if (projectile) {
		projectile->setupVisual();
		prac1Projectiles.push_back(projectile);
		cout << "Proyectil creado - Tipo: " << type
			<< ", Energía cinética: " << projectile->getKineticEnergy() << " J" << endl;
	}
}

void cleanDeadProjectiles() {
	for (auto it = prac1Projectiles.begin(); it != prac1Projectiles.end(); ) {
		if (!(*it)->isAlive()) {
			delete* it;
			it = prac1Projectiles.erase(it);
		}
		else {
			++it;
		}
	}
}

void createGround() {
	PxShape* groundShape = CreateShape(PxBoxGeometry(20, 0.1, 20));
	PxTransform* groundTransform = new PxTransform(PxVec3(0, -2, 0));
	ground = new RenderItem(groundShape, groundTransform, Vector4(0.3, 0.3, 0.3, 1));
}

void createTarget() {
	PxShape* targetShape = CreateShape(PxBoxGeometry(1, 3, 0.1));
	PxTransform* targetTransform = new PxTransform(PxVec3(15, 0, 0));
	target = new RenderItem(targetShape, targetTransform, Vector4(1, 0, 0, 1));
}

*/
// Initialize physics engine
void initPhysics(bool interactive)
{
	/*
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
	*/
	 /*
	PxShape* shShape = CreateShape(PxSphereGeometry(10),gMaterial);
	PxTransform* spTransform = new PxTransform(PxVec3(0,0,0));
	s = new RenderItem(shShape, spTransform, Vector4(0, 0, 1, 1));
	RegisterRenderItem(s);
	*/

//	axisRenderer = new AxisRenderer(gPhysics, gMaterial);
//	axisRenderer->createAxes(3.0f, 1.0f);

//	createGround();
//	createTarget();
	


	sceneManager = new SceneManager();
	sceneManager->initialize();
	
	}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);
/*
	for (auto p : prac1Particles) {
		p->integrate(t);
		Vector3 pos = p->getPos();
		std::cout << "Pos particula: (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;

	}

	for (auto p : prac1Projectiles) {
		p->integrate(t);
		Vector3 pos = p->getPos();
		cout << "Pos proyectil: (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << endl;
	}

	cleanDeadProjectiles();
	*/
	if (sceneManager) {
		sceneManager->update(t);
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
	/*
	if (axisRenderer != nullptr) {
		delete axisRenderer;
		axisRenderer = nullptr;
	}

	for (auto p : prac1Particles) {
		delete p;
	}
	prac1Particles.clear();

	for (auto p : prac1Projectiles) {
		delete p;
	}
	prac1Projectiles.clear();

	if (ground != nullptr) {
		DeregisterRenderItem(ground);
		delete ground;
	}

	if (target != nullptr) {
		DeregisterRenderItem(target);
		delete target;
	}
	*/

	if (sceneManager != nullptr) {
		delete sceneManager;
		sceneManager = nullptr;
	}

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
	/*
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
	case '1':
		createProjectile(1);
		cout << "Disparado: BALA" << endl;
		break;
	case '2':
		createProjectile(2);
		cout << "Disparado: BOLA DE CAÑÓN" << endl;
		break;
	case ' ':
	{
		break;
	}
	default:
		break;
	}
	*/

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
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
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