#include "sherlock.h"
#include "handling.h"
#include "Player.h"
#include "battle.h"
#include<iostream>

using namespace std;


Sherlock::Sherlock()  :Fighter("Sherlock", 16 , 16 ,true,2 , SHERLOCK)
{
    
}
 void Sherlock:: specialAbillity(Battle* battle)
{
}
void Sherlock::setupUnits(Battle* battle, Player& player)
{
    battle->chooseSidekickPosition(player);
}