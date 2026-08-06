#ifndef INVISIBLE_H
#define INVISIBLE_H

#include "fighter.h"
#include "Player.h"

class InvisibleMan : public Fighter
{
public:

    InvisibleMan();
    void specialAbillity(Battle* battle)override;
    void setupUnits(Battle* battle, Player& player) override;

};

#endif