// Created by Patricio Palma on 20-03-26.
#pragma once

#include "ColliderComponent.h"
#include "GameStateManager.h"
#include "GameStats.h"
#include "PlayerController.h"
#include "SceneOrchestrator.h"
#include "ScriptComponent.h"
#include "ShapeComponent.h"
#include "ShieldBunker.h"

#include <algorithm>
#include <vector>

constexpr float     kFireRate         = 0.25f;
constexpr float     kBulletSpeed      = 600.f;
constexpr Vec2F     kBulletSize       {2.f, 10.f};
constexpr float     kPlayerSpeed      = 400.f;
constexpr float     kPlayerHalfW      = 25.f;
constexpr float     kPlayerMargin     = 10.f;
constexpr SDL_Color kMainColor        {0x00, 0xff, 0x00, 0xff};
constexpr int       kPointsPerMartian = 10;

// ---------------------------------------------------------------------------
// ShipController
// Handles player movement, shooting, and bullet-collision logic.
// Shield hit detection is done manually per frame inside each bullet's
// ScriptComponent — no collision system involvement for shields.
// ---------------------------------------------------------------------------
class ShipController : public PlayerController {
public:
    ShipController(float screenWidth,
                   std::vector<ShieldBunker::ShieldBunkerComponent*> shields)
        : PlayerController(0),
          screenWidth_(screenWidth),
          shields_(std::move(shields)) {
        bind("Exit",      SDL_SCANCODE_ESCAPE);
        bind("MoveLeft",  SDL_SCANCODE_LEFT);
        bind("MoveRight", SDL_SCANCODE_RIGHT);
        bind("Fire",      SDL_SCANCODE_SPACE);
    }

    void update(const float dt) override {
        if (!hasPawn()) return;

        if (isActionPressed("Exit"))
            SceneOrchestrator::instance().goTo("menu");

        auto& pos = getPawn()->transform.position;
        pos.x += getMovementAxis().x * kPlayerSpeed * dt;
        pos.x  = std::clamp(pos.x, kPlayerHalfW + kPlayerMargin,
                                   screenWidth_ - kPlayerHalfW - kPlayerMargin);

        fireCooldown_ -= dt;
        if (isActionDown("Fire") && fireCooldown_ <= 0.f) {
            spawnBullet(pos);
            fireCooldown_ = kFireRate;
        }
    }

private:
    void spawnBullet(const Vec2F from) const {
        auto* bullet = getScene()->addObject("Bullet");
        bullet->setTag("Bullet");
        bullet->transform.position = {from.x, from.y - 10.f};
        bullet->addComponent<SquareComponent>("Shape", kBulletSize, kMainColor);

        // Movement + manual shield check every frame.
        // Using the collision system for shields has a fundamental flaw: onEnter fires
        // only once, so a bullet entering through the arch (empty space) would pass
        // through without triggering on subsequent frames.
        bullet->addComponent<ScriptComponent>("Move",
            [shields = shields_](const float dt, Object* self) {
                self->transform.position.y -= kBulletSpeed * dt;
                if (self->transform.position.y < -10.f) { self->destroy(); return; }

                const Vec2F& p = self->transform.position;
                const SDL_FRect bounds {
                    p.x - kBulletSize.x * 0.5f,
                    p.y - kBulletSize.y * 0.5f,
                    kBulletSize.x, kBulletSize.y,
                };
                for (auto* shield : shields) {
                    if (shield->hitBullet(bounds)) { self->destroy(); return; }
                }
            });

        // Collider kept only for martian hits via the collision system.
        auto* collider = bullet->addComponent<BoxColliderComponent>("Collider", kBulletSize);
        collider->onEnter = [thisColl = collider](ColliderComponent* otherColl) {
            if (otherColl->getOwner()->getTag() != "Martian") return;
            GameStateManager::instance().get<GameStats>().score += kPointsPerMartian;
            otherColl->getOwner()->destroy();
            thisColl->getOwner()->destroy();
        };
    }

    float  screenWidth_;
    float  fireCooldown_{0.f};
    std::vector<ShieldBunker::ShieldBunkerComponent*> shields_;
};
