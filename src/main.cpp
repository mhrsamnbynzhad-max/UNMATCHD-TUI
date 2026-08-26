#include "battle.h"
#include "Gamecontroller.h"
#include <cstdlib>
#include <ctime>
#include <optional>
#include "graphic.h"
#include <SFML/Audio.hpp>

int main()
{
    srand(time(nullptr));

 //   Battle battle;

   /* GameController game(
        battle,
        battle.getplayer1(),
        battle.getplayer2()
    );*/

    sf::Music music;

    if (!music.openFromFile("background.mp3"))
    {
        return -1;
    }

    music.setLooping(true);
    music.setVolume(30);
    music.play();

    GraphicManager graphic;
    graphic.run();

    return 0;
}