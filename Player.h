#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <stdexcept>
#include "fighter.h"

class Battle;

class Player
{
private:
    std::string name;
    Fighter* hero;

public:
    Player(const std::string& n);

    void chooseHero(Fighter* sherlock, Fighter* dracula);

    Fighter* getHero() const;
    void setHero(Fighter* );
    std::string getName() const;

    void drawCard();
    void maneuver(Battle& battle);
    void attack(Player& enemy, Battle& battle , int);
    void playScheme(Player& , Battle& , int );
};

#endif