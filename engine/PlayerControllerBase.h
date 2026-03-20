// Created by Patricio Palma on 20-03-26.

#ifndef LETSLEARNSDL_PLAYERCONTROLLERBASE_H
#define LETSLEARNSDL_PLAYERCONTROLLERBASE_H

#include "Object.h"

class Scene;

// Abstract base class. Decouples a pawn (Object) from whoever drives it —
// a human player or an AI. Subclass PlayerController or AIController.
class PlayerControllerBase {
public:
    virtual ~PlayerControllerBase() = default;

    PlayerControllerBase(const PlayerControllerBase&)            = delete;
    PlayerControllerBase& operator=(const PlayerControllerBase&) = delete;
    PlayerControllerBase(PlayerControllerBase&&)                 = delete;
    PlayerControllerBase& operator=(PlayerControllerBase&&)      = delete;

    // Possess a pawn. Fires onUnpossess on the previous pawn (if any),
    // then onPossess on the new one. Passing nullptr is equivalent to unpossess().
    void possess(Object* pawn) {
        if (pawn_ == pawn) return;
        if (pawn_) onUnpossess(pawn_);
        pawn_ = pawn;
        if (pawn_) onPossess(pawn_);
    }

    void unpossess() {
        if (!pawn_) return;
        onUnpossess(pawn_);
        pawn_ = nullptr;
    }

    Object* getPawn()  const { return pawn_; }
    bool    hasPawn()  const { return pawn_ != nullptr; }
    Scene*  getScene() const { return scene_; }

    // Called once per frame by the Scene, before component updates.
    virtual void update(float dt) = 0;

protected:
    PlayerControllerBase() = default;

    // Override to react when this controller takes or releases control of a pawn.
    virtual void onPossess(Object* /*pawn*/)   {}
    virtual void onUnpossess(Object* /*pawn*/) {}

private:
    Object* pawn_{nullptr};
    Scene*  scene_{nullptr};

    friend class Scene;
};

#endif //LETSLEARNSDL_PLAYERCONTROLLERBASE_H