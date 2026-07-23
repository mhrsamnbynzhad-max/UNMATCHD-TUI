#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "battle.h"
#include "player.h"
#include <queue>

class GameController
{
private:
    Battle& battle;
    Player& player1;
    Player& player2;

public:
    GameController(Battle& b, Player& p1, Player& p2);

    void run();
};

#endif