// Created by Patricio Palma on 20-03-26.

#include "GameEngine.h"
#include "SceneOrchestrator.h"
#include "MenuScene.h"
#include "PongScene.h"
#include "GameOverScene.h"
#include <iostream>

int main() {
    GameEngine& engine = GameEngine::instance();
    if (!engine.init("Pong", 800, 600)) {
        std::cerr << "Cannot initialize the game.\n";
        return EXIT_FAILURE;
    }

    auto& orch = SceneOrchestrator::instance();
    orch.registerScene<MenuScene>("menu");
    orch.registerScene<PongScene>("pong");
    orch.registerScene<GameOverScene>("gameover");

    orch.start("menu");

    engine.run();
    engine.shutdown();
    return EXIT_SUCCESS;
}
