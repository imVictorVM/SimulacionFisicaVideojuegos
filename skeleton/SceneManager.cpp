#include "SceneManager.h"
#include "Scene0.h"
#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"
#include "Scene4.h"
#include "Scene5.h"
#include <iostream>

extern std::string display_text;
extern std::string ui_text;

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

    display_text = "";
    ui_text = "";

    switch (scene) {
    case SCENE_0:
        currentScene = std::make_unique<Scene0>();
        break;
    case SCENE_1:
        currentScene = std::make_unique<Scene1>();
        break;
    case SCENE_2:
        currentScene = std::make_unique<Scene2>();
        break;
    case SCENE_3:
        currentScene = std::make_unique<Scene3>();
        break;
    case SCENE_4:
        currentScene = std::make_unique<Scene4>();
        break;
    case SCENE_5:
        currentScene = std::make_unique<Scene5>();
        display_text = "+";
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