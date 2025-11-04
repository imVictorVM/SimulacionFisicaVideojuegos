#include "Scene2.h"
#include <iostream>
#include "Render/Camera.h"

Scene2::Scene2() : _particleSystem(nullptr), fountainGenerator(nullptr),
smokeGenerator(nullptr),
sparksGenerator(nullptr) {
}

Scene2::~Scene2() {
    cleanup();
}

void Scene2::initialize() {
    std::cout << "Inicializando " << getDescription() << std::endl;

    Camera* cam = GetCamera();
    if (cam) {
        original_cam_eye = cam->getEye();
        original_cam_dir = cam->getDir();
        cam->setEye({ 0, 7, 35 });
        cam->setDir({ 0, -0.2f, -1.0f });
    }

    _particleSystem = new ParticleSystem();

    // --- EFECTO 1: Fuente de Agua ---
    fountainGenerator = new GaussianGenerator("Fountain");
    Particle* water_model = new Particle({ 0,0,0 }, { 0,0,0 }, 1.0, 0.99, 5.0);
    water_model->setupVisual();
    water_model->setColor({ 0.2, 0.5, 1.0, 1.0 });
    fountainGenerator->setParticleModel(water_model);
    fountainGenerator->setMeanPosition({ 0, 2, 15 });
    fountainGenerator->setMeanVelocity({ 0, 15, 0 });
    fountainGenerator->setPosDeviation({ 0.1, 0, 0.1 });
    fountainGenerator->setVelDeviation({ 3, 3, 3 });
    fountainGenerator->setNumParticles(10);
    _particleSystem->addGenerator(fountainGenerator);

    // --- EFECTO 2: Humo ---
    smokeGenerator = new UniformGenerator("Smoke");
    Particle* smoke_model = new Particle({ 0,0,0 }, { 0,0,0 }, 0.1, 0.95, 10.0);
    smoke_model->setupVisual();
    smoke_model->setColor({ 0.8, 0.8, 0.8, 0.4 });
    smokeGenerator->setParticleModel(smoke_model);
    smokeGenerator->setPosRange({ 10, 0, 10 }, { 20, 2, 20 });
    smokeGenerator->setVelRange({ -1, 0.5, -1 }, { 1, 2, 1 });
    smokeGenerator->setNumParticles(5);
    _particleSystem->addGenerator(smokeGenerator);

    // --- EFECTO 3: Chispas ---
    sparksGenerator = new GaussianGenerator("Sparks");
    Particle* spark_model = new Particle({ 0,0,0 }, { 0,0,0 }, 0.5, 0.8, 1.5);
    spark_model->setupVisual();
    spark_model->setColor({ 1.0, 0.5, 0.0, 1.0 });
    sparksGenerator->setParticleModel(spark_model);
    sparksGenerator->setMeanPosition({ -15, 1, 15 });
    sparksGenerator->setMeanVelocity({ 0, 5, 0 });
    sparksGenerator->setVelDeviation({ 4, 4, 4 });
    sparksGenerator->setNumParticles(3);
    _particleSystem->addGenerator(sparksGenerator);
}

void Scene2::update(double t) {
    if (_particleSystem) {
        _particleSystem->update(t);
    }
}

void Scene2::cleanup() {
    Camera* cam = GetCamera();
    if (cam) {
        cam->setEye(original_cam_eye);
        cam->setDir(original_cam_dir);
    }
    if (_particleSystem) {
        delete _particleSystem;
        _particleSystem = nullptr;
    }
}

void Scene2::handleKeyPress(unsigned char key) {
    switch (toupper(key)) {
    case 'F':
        if (fountainGenerator) fountainGenerator->toggleActive();
        break;
    case 'H':
        if (smokeGenerator) smokeGenerator->toggleActive();
        break;
    case 'C':
        if (sparksGenerator) sparksGenerator->toggleActive();
        break;
    }
}