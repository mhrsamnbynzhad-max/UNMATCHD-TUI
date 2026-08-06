#include "invisible.h"
#include "battle.h"

InvisibleMan :: InvisibleMan() : Fighter("InvisibleMan", 15 , 15 , false , 2 , INVISIBLE)
{
}

void InvisibleMan::setupUnits(Battle* battle, Player& player)
{
    battle->chooseFogPosition(player);
}

void InvisibleMan::specialAbillity(Battle* battle)
{
    if(battle->getfogtoken().empty())
        return;

    for(FogToken& fog : battle->getfogtoken())
    {
        if(fog.getPosition() == getPosition())
        {
            battle->setinvisibleability(true);
            return;
        }
    }

    battle->setinvisibleability(false);
}

