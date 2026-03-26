// Created by Patricio Palma on 20-03-26.

#include "SpaceInvadersScene.h"

#include "ColliderComponent.h"
#include "CollisionSystem.h"
#include "FontManager.h"
#include "GameStateManager.h"
#include "GameStats.h"
#include "MartianMoveComponent.h"
#include "SceneOrchestrator.h"
#include "ScriptComponent.h"
#include "ShapeComponent.h"
#include "ShieldBunker.h"
#include "ShipController.h"
#include "TextComponent.h"

#include <format>

namespace {

constexpr SDL_Color kBackgroundColor {0x1a, 0x1a, 0x2e, 0xff};
constexpr Vec2F     kPlayerSize      {40.f, 15.f};

// Martians
constexpr Vec2F     kMartianSize    {33.f, 30.f};
constexpr int       kEnemiesPerLine = 11;
constexpr int       kEnemyLines     = 5;
constexpr float     kEnemiesStep    = kMartianSize.x * 1.2f;
constexpr float     kEnemiesStart   = 40.f;
constexpr SDL_Color kMartianColor   {0xff, 0xff, 0xff, 0xff};

// Shields
constexpr int   kShieldCount  = 4;
constexpr float kShieldMargin = 110.f;

} // namespace

// ---------------------------------------------------------------------------
// SpaceInvadersScene::load
// ---------------------------------------------------------------------------
void SpaceInvadersScene::load() {
    addSubsystem<CollisionSystem>();
    const GameStats& stats = GameStateManager::instance().get<GameStats>();

    setBackgroundColor(kBackgroundColor);
    TTF_Font* fontScore = FontManager::instance().load("assets/VT323-Regular.ttf", 24.f);
    auto& [sw, sh] = getScreenSize();

    // Player
    auto* player = addObject("Player");
    player->transform.position = {sw * 0.5f, sh - 90.f};
    player->addComponent<SquareComponent>("Shape", kPlayerSize, kMainColor);

    // HUD
    constexpr float uiRow1 = 25.f;
    constexpr float uiRow2 = uiRow1 + 28.f;
    auto* ui = addObject("UI");
    ui->addComponent<TextComponent>("TextScore1Hdr", "SCORE<1>", fontScore, kMainColor,
        Vec2F{sw * 0.25f, uiRow1});
    ui->addComponent<TextComponent>("TextHScoreHdr", "HI-SCORE", fontScore, kMainColor,
        Vec2F{sw * 0.50f, uiRow1});
    ui->addComponent<TextComponent>("TextScore2Hdr", "SCORE<2>", fontScore, kMainColor,
        Vec2F{sw * 0.75f, uiRow1});

    auto* scoreText = ui->addComponent<TextComponent>("TextScore1",
        std::format("{:05}", stats.score), fontScore, kMainColor, Vec2F{sw * 0.25f, uiRow2});

    ui->addComponent<ScriptComponent>("ScoreUpdater",
        [scoreText, prev = stats.score](float, Object*) mutable {
            const int current = GameStateManager::instance().get<GameStats>().score;
            if (current == prev) return;
            prev = current;
            scoreText->setText(std::format("{:05}", current));
        });

    ui->addComponent<TextComponent>("TextHiScore", "00000", fontScore, kMainColor,
        Vec2F{sw * 0.50f, uiRow2});
    ui->addComponent<TextComponent>("TextScore2", "00000", fontScore, kMainColor,
        Vec2F{sw * 0.75f, uiRow2});
    ui->addComponent<TextComponent>("TxtCredit", "CREDIT 00", fontScore, kMainColor,
        Vec2F{sw * 0.85f, 760.f});

    // Shields — created before the controller so the bullet scripts can reference them.
    std::vector<ShieldBunker::ShieldBunkerComponent*> shields;
    for (int i = 0; i < kShieldCount; ++i) {
        const float cx = kShieldMargin + (sw - 2 * kShieldMargin) * (i / 3.f);
        shields.push_back(ShieldBunker::create(this, i, {cx, 650.f}, kMainColor));
    }

    addPlayerController<ShipController>(static_cast<float>(sw), std::move(shields))->possess(player);

    // Martians
    std::vector<Object*> martians;
    float base = 300.f;
    for (int i = 0; i < kEnemyLines; ++i) {
        for (int j = 0; j < kEnemiesPerLine; ++j) {
            auto* m = addObject(std::format("Martian.{}.{}", i, j));
            m->transform.position = {kEnemiesStart + kEnemiesStep * j, base};
            m->setTag("Martian");
            m->setActive(false);
            m->addComponent<SquareComponent>("Shape", kMartianSize, kMartianColor);
            auto* collider = m->addComponent<CircleColliderComponent>("Collider", kMartianSize.x / 2.f - 1.f);
            collider->debugDraw = true;
            martians.push_back(m);
        }
        base -= kMartianSize.y + 10.f;
    }
    addObject("MoveMartians")->addComponent<MartianMoveComponent>(
        "TheMoverCompo",
        std::move(martians),
        static_cast<float>(sw),
        [] { SceneOrchestrator::instance().goTo("thegame"); }
    );

    // Bottom line
    auto* line = addObject("LineObject");
    line->transform.position = {sw * 0.5f, 750.f};
    line->addComponent<SquareComponent>("Line", Vec2F{static_cast<float>(sw), 1.f}, kMainColor);
}
