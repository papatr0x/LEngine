// Created by Patricio Palma on 20-03-26.

#ifndef LETSLEARNSDL_AICONTROLLER_H
#define LETSLEARNSDL_AICONTROLLER_H

#include "PlayerControllerBase.h"

// Base class for AI-driven controllers.
// Subclass this and implement update() with your AI logic.
// Use getPawn() to read/write the possessed object's state.
class AIController : public PlayerControllerBase {
public:
    ~AIController() override = default;

protected:
    AIController() = default;
};

#endif //LETSLEARNSDL_AICONTROLLER_H