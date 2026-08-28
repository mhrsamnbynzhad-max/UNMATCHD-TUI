#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <stdexcept>
#include "fighter.h"

class Battle;
class Fighter;

class Player
{
private:
    std::string name;
    Fighter* hero;

public:
    Player(const std::string& n);

    void chooseHero(const vector<Fighter*>& heroes);
    Fighter* getHero() const;
    void setHero(Fighter* );
    std::string getName() const;

    void drawCard();
    void maneuver(Battle& battle);
    void attack(Player& , Battle& , Fighter*, int);
    void playScheme(Player& , Battle& ,Fighter*, int );
    Fighter* chooseAttacker( Battle& , Card& , Fighter*);
    bool chooseAttackerIfNeeded (Battle& , Card& , Fighter*& ,Fighter* );
    vector<Fighter*> getAttackerChoices(Battle&, Card&, Fighter*);
};

#endif