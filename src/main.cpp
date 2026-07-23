#include "battle.h"
#include "Gamecontroller.h"
#include <cstdlib>
#include <ctime>
#include "StatusPanel.h"



int main()
{
   
   srand(time(nullptr));
   Battle battle;
   
   GameController game (battle ,battle.getplayer1() ,battle.getplayer2());
   game.run();
   return 0;
}