#pragma once
#include <map>
#include <memory>
#include "Scene.h"

class SceneManager {
public:
    enum SceneType {
        SCENE_0 = 0,
        SCENE_1 = 1,
        SCENE_2 = 2
        // Puedes a�adir m�s escenas aqu�
    };

    SceneManager();
    ~SceneManager();

    void initialize();
    void cleanup();

    void switchToScene(SceneType scene);
    void update(double t);
    void handleKeyPress(unsigned char key);

    Scene* getCurrentScene() { return currentScene.get(); }
    SceneType getCurrentSceneType() const { return currentSceneType; }

private:
    void cleanupCurrentScene();

    std::unique_ptr<Scene> currentScene;
    SceneType currentSceneType;
};