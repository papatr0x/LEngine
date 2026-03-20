// Created by Patricio Palma on 20-03-26.

#ifndef LETSLEARNSDL_PONGSTATE_H
#define LETSLEARNSDL_PONGSTATE_H

// Shared match state. Written by BallController, read by GameOverScene.
struct PongState {
    static inline int  scoreP1{0};
    static inline int  scoreP2{0};
    static inline int  winner{0};   // 1 = Player, 2 = AI

    static constexpr int winScore{10};

    static void reset() { scoreP1 = scoreP2 = winner = 0; }
};

#endif //LETSLEARNSDL_PONGSTATE_H
