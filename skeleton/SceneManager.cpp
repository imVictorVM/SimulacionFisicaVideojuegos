#include "SceneManager.h"
#include "Scene0.h"
#include "Scene1.h"
#include <iostream>

SceneManager::SceneManager()
    : currentSceneType(SCENE_0)
{
}

SceneManager::~SceneManager() {
    cleanup();
}

void SceneManager::initialize() {
    // Inicializar con la escena 0 por defecto
    switchToScene(SCENE_0);
}

void SceneManager::cleanup() {
    cleanupCurrentScene();
}

void SceneManager::cleanupCurrentScene() {
    if (currentScene) {
        currentScene->cleanup();
        currentScene.reset();
    }
}

void SceneManager::switchToScene(SceneType scene) {
    cleanupCurrentScene();
    currentSceneType = scene;

    switch (scene) {
    case SCENE_0:
        currentScene = std::make_unique<Scene0>();
        break;
    case SCENE_1:
        currentScene = std::make_unique<Scene1>();
        break;
    }

    if (currentScene) {
        currentScene->initialize();
        std::cout << currentScene->getDescription() << std::endl;
    }
}

void SceneManager::update(double t) {
    if (currentScene) {
        currentScene->update(t);
    }
}

void SceneManager::handleKeyPress(unsigned char key) {
    if (currentScene) {
        currentScene->handleKeyPress(key);
    }
}