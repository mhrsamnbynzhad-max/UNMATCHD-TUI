#ifndef SHERLOCK_H
#define SHERLOCK_H

#include "fighter.h"
#include "player.h"

class Sherlock : public Fighter
{
public:

    Sherlock();

    void specialAbillity(Battle* battle)override;
    void setupUnits(Battle* battle, Player& player) override;

};

#endif