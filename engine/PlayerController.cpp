// Created by Patricio Palma on 20-03-26.

#include "PlayerController.h"
#include "GameEngine.h"
#include <cmath>

PlayerController::PlayerController(int playerIndex)
    : playerIndex_(playerIndex), input_(GameEngine::instance().getInput())
{
    setupDefaultBindings();
}

void PlayerController::setupDefaultBindings() {
    if (playerIndex_ == 0) {
        actions_.bind("MoveRight", SDL_SCANCODE_D);
        actions_.bind("MoveLeft",  SDL_SCANCODE_A);
        actions_.bind("MoveDown",  SDL_SCANCODE_S);
        actions_.bind("MoveUp",    SDL_SCANCODE_W);
        actions_.bind("Jump",      SDL_SCANCODE_SPACE);
        actions_.bind("Fire",      SDL_SCANCODE_LCTRL);
        actions_.bind("Jump",      SDL_GAMEPAD_BUTTON_SOUTH);
        actions_.bind("Fire",      SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER);
    } else {
        actions_.bind("MoveRight", SDL_SCANCODE_RIGHT);
        actions_.bind("MoveLeft",  SDL_SCANCODE_LEFT);
        actions_.bind("MoveDown",  SDL_SCANCODE_DOWN);
        actions_.bind("MoveUp",    SDL_SCANCODE_UP);
        actions_.bind("Jump",      SDL_SCANCODE_RETURN);
        actions_.bind("Fire",      SDL_SCANCODE_RCTRL);
    }
}

Vec2F PlayerController::getMovementAxis() const {
    Vec2F axis{};

    if (isActionDown("MoveRight")) axis.x += 1.0f;
    if (isActionDown("MoveLeft"))  axis.x -= 1.0f;
    if (isActionDown("MoveDown"))  axis.y += 1.0f;
    if (isActionDown("MoveUp"))    axis.y -= 1.0f;

    // Gamepad left stick — only P0, the engine manages a single gamepad.
    // The stick overrides the keyboard component when its magnitude is larger,
    // so both inputs can coexist without summing.
    if (playerIndex_ == 0 && input_.isGamepadConnected()) {
        const float gx = input_.getAxis(SDL_GAMEPAD_AXIS_LEFTX);
        const float gy = input_.getAxis(SDL_GAMEPAD_AXIS_LEFTY);
        if (std::abs(gx) > std::abs(axis.x)) axis.x = gx;
        if (std::abs(gy) > std::abs(axis.y)) axis.y = gy;
    }

    return axis;
}