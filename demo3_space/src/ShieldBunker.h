// Created by Patricio Palma on 20-03-26.
#pragma once

#include "Component.h"
#include "Object.h"
#include "Scene.h"
#include "Vector.h"

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <algorithm>
#include <cmath>
#include <format>

// ---------------------------------------------------------------------------
// ShieldBunker
// Pixel-destructible bunker.  A single BoxCollider covers the full AABB;
// ShieldBunkerComponent maintains the pixel grid and renders it directly.
// This avoids spawning hundreds of per-pixel Objects and Colliders.
// ---------------------------------------------------------------------------
namespace ShieldBunker {

constexpr float kPixelSize = 2.f;

// Classic 33×24 bitmap (2 px/pixel → 66×48 px on screen).
// 'X' = filled, '.' = empty.  Every row must be exactly kCols characters wide.
static constexpr const char* kBitmap[] = {
    "...XXXXXXXXXXXXXXXXXXXXXXXXXXX...",
    "..XXXXXXXXXXXXXXXXXXXXXXXXXXXXX..",
    ".XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX.",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "XXXXXXXXXXXX.........XXXXXXXXXXXX",
    "XXXXXXXXXXX...........XXXXXXXXXXX",
    "XXXXXXXXXXX...........XXXXXXXXXXX",
    "XXXXXXXXXXX...........XXXXXXXXXXX",
};
constexpr int kCols = 33;
constexpr int kRows = 24;

// ---------------------------------------------------------------------------
class ShieldBunkerComponent : public Component {
public:
    ShieldBunkerComponent(Object* owner, const std::string& id, const SDL_Color color)
        : Component(owner, id), color_(color) {
        for (int row = 0; row < kRows; ++row)
            for (int col = 0; col < kCols; ++col)
                pixels_[row][col] = (kBitmap[row][col] == 'X');
    }

    // Destroys every pixel that overlaps `bulletBounds`.
    // Returns true if at least one pixel was hit.
    bool hitBullet(const SDL_FRect& bulletBounds) {
        const Vec2F tl = topLeft();
        const int c0 = std::max(0,        static_cast<int>(std::floor((bulletBounds.x                  - tl.x) / kPixelSize)));
        const int c1 = std::min(kCols - 1,static_cast<int>(std::floor((bulletBounds.x + bulletBounds.w - tl.x) / kPixelSize)));
        const int r0 = std::max(0,        static_cast<int>(std::floor((bulletBounds.y                  - tl.y) / kPixelSize)));
        const int r1 = std::min(kRows - 1,static_cast<int>(std::floor((bulletBounds.y + bulletBounds.h - tl.y) / kPixelSize)));

        bool hit = false;
        for (int r = r0; r <= r1; ++r)
            for (int c = c0; c <= c1; ++c)
                if (pixels_[r][c]) { pixels_[r][c] = false; hit = true; }
        return hit;
    }

    void render(SDL_Renderer* renderer) noexcept override {
        SDL_SetRenderDrawColor(renderer, color_.r, color_.g, color_.b, color_.a);
        const Vec2F tl  = topLeft();
        const Vec2F off = screenSpace ? Vec2F{} : owner->getScene()->getWorldRenderOffset();
        for (int r = 0; r < kRows; ++r) {
            for (int c = 0; c < kCols; ++c) {
                if (!pixels_[r][c]) continue;
                const SDL_FRect rect {
                    tl.x + static_cast<float>(c) * kPixelSize + off.x,
                    tl.y + static_cast<float>(r) * kPixelSize + off.y,
                    kPixelSize, kPixelSize,
                };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

private:
    Vec2F topLeft() const {
        return {
            owner->transform.position.x - kCols * kPixelSize * 0.5f,
            owner->transform.position.y - kRows * kPixelSize * 0.5f,
        };
    }

    bool      pixels_[kRows][kCols]{};
    SDL_Color color_;
};

// ---------------------------------------------------------------------------
// Creates a bunker centred at `center` and returns its component.
// No collider is added — hit detection is handled manually by the bullet each frame,
// so bullets can pass through the arch without phantom stops.
inline ShieldBunkerComponent* create(Scene* scene, const int id,
                                     const Vec2F center, const SDL_Color color) {
    auto* shield = scene->addObject(std::format("Shield{}", id));
    shield->transform.position = center;
    return shield->addComponent<ShieldBunkerComponent>("Pixels", color);
}

} // namespace ShieldBunker
