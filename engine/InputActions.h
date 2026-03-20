// Created by Patricio Palma on 07-03-26.

#ifndef LETSLEARNSDL_INPUTACTIONS_H
#define LETSLEARNSDL_INPUTACTIONS_H
#include "Input.h"
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>

struct ActionBindings {
    std::vector<SDL_Scancode>      keys;
    std::vector<SDL_GamepadButton> buttons;
};

class InputActions {
public:
    // Keyboard bindings
    void bind(std::string_view action, SDL_Scancode scancode) {
        bindings[std::string(action)].keys.push_back(scancode);
    }

    void unbind(std::string_view action, SDL_Scancode scancode) {
        const auto it = bindings.find(action);
        if (it == bindings.end()) return;
        std::erase(it->second.keys, scancode);
    }

    // Gamepad bindings
    void bind(std::string_view action, SDL_GamepadButton button) {
        bindings[std::string(action)].buttons.push_back(button);
    }

    void unbind(std::string_view action, SDL_GamepadButton button) {
        const auto it = bindings.find(action);
        if (it == bindings.end()) return;
        std::erase(it->second.buttons, button);
    }

    void unbindAll(std::string_view action) {
        bindings.erase(std::string(action));
    }

    const ActionBindings* getBindings(std::string_view action) const {
        const auto it = bindings.find(action);
        return it != bindings.end() ? &it->second : nullptr;
    }

    // Query helpers — centralise the loop so neither InputComponent
    // nor PlayerController need to duplicate it.
    bool isDown(const Input& input, std::string_view action) const {
        const auto* b = getBindings(action);
        if (!b) return false;
        for (const auto key : b->keys)    if (input.isKeyDown(key))             return true;
        for (const auto btn : b->buttons)  if (input.isGamepadButtonDown(btn))   return true;
        return false;
    }

    bool isPressed(const Input& input, std::string_view action) const {
        const auto* b = getBindings(action);
        if (!b) return false;
        for (const auto key : b->keys)    if (input.isKeyPressed(key))          return true;
        for (const auto btn : b->buttons)  if (input.isGamepadButtonPressed(btn)) return true;
        return false;
    }

    bool isReleased(const Input& input, std::string_view action) const {
        const auto* b = getBindings(action);
        if (!b) return false;
        for (const auto key : b->keys)    if (input.isKeyReleased(key))          return true;
        for (const auto btn : b->buttons)  if (input.isGamepadButtonReleased(btn)) return true;
        return false;
    }

private:
    struct StringHash {
        using is_transparent = void;
        size_t operator()(std::string_view sv) const {
            return std::hash<std::string_view>{}(sv);
        }
    };
    std::unordered_map<std::string, ActionBindings, StringHash, std::equal_to<>> bindings;
};

#endif //LETSLEARNSDL_INPUTACTIONS_H
