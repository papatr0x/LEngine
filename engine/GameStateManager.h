// Created by Patricio Palma on 24-03-26.

#pragma once
#include <any>
#include <typeindex>
#include <unordered_map>

/**
 * Persistent singleton that stores typed state objects shared across scenes.
 *
 * Usage:
 *   GameStateManager::instance().set(MyState{});
 *   auto& s = GameStateManager::instance().get<MyState>();
 *   s.score += 100;
 */
class GameStateManager {
public:
    GameStateManager(const GameStateManager&)            = delete;
    GameStateManager& operator=(const GameStateManager&) = delete;

    static GameStateManager& instance() {
        static GameStateManager inst;
        return inst;
    }

    /** Store or replace a state of type T. */
    template<typename T>
    void set(T state) {
        states[typeid(T)] = std::move(state);
    }

    /** Get a reference to the stored T. Throws std::bad_any_cast if not set. */
    template<typename T>
    T& get() {
        return std::any_cast<T&>(states.at(typeid(T)));
    }

    /** Get a const reference to the stored T. Throws std::bad_any_cast if not set. */
    template<typename T>
    const T& get() const {
        return std::any_cast<const T&>(states.at(typeid(T)));
    }

    /** Returns a pointer to T, or nullptr if not set. Never throws. */
    template<typename T>
    T* tryGet() {
        auto it = states.find(typeid(T));
        if (it == states.end()) return nullptr;
        return std::any_cast<T>(&it->second);
    }

    /** Returns true if a state of type T has been set. */
    template<typename T>
    bool has() const {
        return states.count(typeid(T)) > 0;
    }

    /** Remove the state of type T. No-op if not present. */
    template<typename T>
    void reset() {
        states.erase(typeid(T));
    }

    /** Remove all stored states. Called by GameEngine::shutdown(). */
    void clear() {
        states.clear();
    }

private:
    GameStateManager() = default;
    std::unordered_map<std::type_index, std::any> states;
};