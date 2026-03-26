// Created by Patricio Palma on 20-03-26.
#pragma once

#include "Component.h"
#include "Object.h"

#include <algorithm>
#include <functional>
#include <vector>

constexpr float kPauseInterval = 0.5f;
constexpr float kStepInterval  = 0.01f;
constexpr float kStepX         = 8.f;
constexpr float kStepY         = 20.f;
constexpr float kBorder        = 20.f;

// ---------------------------------------------------------------------------
// MartianMoveComponent
// Drives the classic Space Invaders sweeping formation movement.
// Moves one martian per step across the formation; descends and reverses
// direction whenever the leading edge reaches a screen border.
//
// `onWaveCleared` is invoked (if set) when all martians have been destroyed.
// ---------------------------------------------------------------------------
class MartianMoveComponent : public Component {
public:
    MartianMoveComponent(Object* owner, const std::string& id,
                         std::vector<Object*> martians,
                         float screenWidth,
                         std::function<void()> onWaveCleared = {})
        : Component(owner, id),
          martians_(std::move(martians)),
          screenWidth_(screenWidth),
          onWaveCleared_(std::move(onWaveCleared)) {}

    void updateComponent(const float dt) noexcept override {
        // Adjust cursor before erasing so it stays aligned after index shifts.
        int removedBeforeCursor = 0;
        for (int i = 0; i < cursor_ && i < static_cast<int>(martians_.size()); ++i) {
            if (martians_[i]->isPendingDestroy())
                ++removedBeforeCursor;
        }
        std::erase_if(martians_, [](const Object* m) { return m->isPendingDestroy(); });
        cursor_ = std::max(0, cursor_ - removedBeforeCursor);

        if (martians_.empty()) {
            if (onWaveCleared_) onWaveCleared_();
            return;
        }

        if (waiting_) {
            pauseAccum_ += dt;
            if (pauseAccum_ < kPauseInterval) return;
            pauseAccum_ = 0.f;
            waiting_    = false;
        }

        stepAccum_ += dt;
        if (stepAccum_ < kStepInterval) return;
        stepAccum_ = 0.f;

        if (cursor_ >= static_cast<int>(martians_.size()))
            cursor_ = 0;

        martians_[cursor_]->transform.position.x += kStepX * dir_;
        martians_[cursor_]->setActive(true);
        ++cursor_;

        if (cursor_ >= static_cast<int>(martians_.size())) {
            cursor_ = 0;

            float minX = martians_[0]->transform.position.x;
            float maxX = martians_[0]->transform.position.x;
            for (const auto* m : martians_) {
                minX = std::min(minX, m->transform.position.x);
                maxX = std::max(maxX, m->transform.position.x);
            }

            if (minX + kStepX * dir_ < kBorder || maxX + kStepX * dir_ > screenWidth_ - kBorder) {
                for (auto* m : martians_)
                    m->transform.position.y += kStepY;
                dir_ = -dir_;
            }

            waiting_ = true;
        }
    }

private:
    std::vector<Object*>  martians_;
    float                 screenWidth_;
    std::function<void()> onWaveCleared_;
    float                 dir_{1.f};
    int                   cursor_{0};
    bool                  waiting_{true};
    float                 pauseAccum_{0.f};
    float                 stepAccum_{0.f};
};
