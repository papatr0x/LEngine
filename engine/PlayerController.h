// Created by Patricio Palma on 20-03-26.

#ifndef LETSLEARNSDL_PLAYERCONTROLLER_H
#define LETSLEARNSDL_PLAYERCONTROLLER_H

#include "PlayerControllerBase.h"
#include "InputActions.h"   // brings Input.h + Vec2F transitively

// Human player controller with pre-mapped action bindings.
//
// Default bindings by playerIndex:
//   P0 — WASD + Space (Jump) + LCtrl (Fire) + gamepad left stick + South / RShoulder
//   P1 — Arrow keys + Enter (Jump) + RCtrl (Fire)
//
// Pre-mapped action names: "MoveRight" "MoveLeft" "MoveUp" "MoveDown" "Jump" "Fire"
//
// Subclass this, implement update(), and use:
//   getMovementAxis()            — combined keyboard + gamepad Vec2F
//   isActionDown/Pressed/Released() — action query via InputActions
//   getInput()                   — raw Input for anything else
//   bind / unbind / unbindAll    — remap or extend defaults at any time
class PlayerController : public PlayerControllerBase {
public:
    explicit PlayerController(int playerIndex = 0);

    int getPlayerIndex() const { return playerIndex_; }

    // Remap or extend the default bindings at any time.
    void bind(std::string_view action, SDL_Scancode key)        { actions_.bind(action, key);    }
    void bind(std::string_view action, SDL_GamepadButton btn)   { actions_.bind(action, btn);    }
    void unbind(std::string_view action, SDL_Scancode key)      { actions_.unbind(action, key);  }
    void unbind(std::string_view action, SDL_GamepadButton btn) { actions_.unbind(action, btn);  }
    void unbindAll(std::string_view action)                     { actions_.unbindAll(action);    }

protected:
    // Combined movement from MoveRight/Left/Up/Down actions + gamepad left stick (P0 only).
    // X: right = +1, left = -1  |  Y: down = +1, up = -1  (screen-space convention).
    // Gamepad overrides the keyboard component when its magnitude is larger.
    Vec2F getMovementAxis() const;

    bool isActionDown(std::string_view action)     const { return actions_.isDown(input_, action);     }
    bool isActionPressed(std::string_view action)  const { return actions_.isPressed(input_, action);  }
    bool isActionReleased(std::string_view action) const { return actions_.isReleased(input_, action); }

    // Raw access for anything not covered by action bindings (mouse, individual axes, etc.).
    const Input& getInput() const { return input_; }

private:
    void setupDefaultBindings();

    Input&       input_;
    InputActions actions_;
    int          playerIndex_;
};

#endif //LETSLEARNSDL_PLAYERCONTROLLER_H