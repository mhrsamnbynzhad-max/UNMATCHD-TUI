#include "battle.h"
#include "Gamecontroller.h"
#include <cstdlib>
#include <ctime>
#include <optional>
#include "graphic.h"

int main()
{
    srand(time(nullptr));

 //   Battle battle;

   /* GameController game(
        battle,
        battle.getplayer1(),
        battle.getplayer2()
    );*/

    GraphicManager graphic;
    graphic.run();

    return 0;
}