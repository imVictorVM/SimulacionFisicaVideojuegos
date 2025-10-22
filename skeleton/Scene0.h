#pragma once
#include "Scene.h"

class AxisRenderer;

class Scene0 : public Scene {
public:
    void initialize() override;
    void cleanup() override;
    void handleKeyPress(unsigned char key) override;
    std::string getDescription() const override { return "Escena 0"; }

private:
    AxisRenderer* axisRenderer = nullptr;

};