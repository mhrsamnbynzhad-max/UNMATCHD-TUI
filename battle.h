#ifndef BATTLE_H
#define BATTLE_H

#include "Player.h"
#include "fighter.h"
#include "dracula.h"
#include "watson.h"
#include "sherlock.h"
#include "map.h"
#include <vector>

class Card;
class CardEffect;
struct ZoneCheckResult
{
   bool allow ;
   std :: string  blocker = "";
};
class Battle
{
private:

    Sherlock sherlock;
    Watson watson;
    Dracula dracula;

    std::vector<Fighter> sisters;

    Map map;
    Player player1;
    Player player2;

    bool canreach(Zone*  , Zone*  , int  , Fighter&);

public:

    Battle();
    void startGame();
    void setuppositions();

    void draculaability(Fighter* target);

    void printfighters();

   
    void combat(Fighter*,Fighter*  );

    void applycardeffect(Card& , Fighter* ,Fighter* );


    bool areadjacent(Fighter& ,Fighter&  );

    bool movefighter(Fighter& ,int  , int );
    void showPossiblemoves(Fighter&);
    Fighter* getfighterat(Zone* );
    void showplacementzone(Fighter& );
    std::vector<Fighter>& getsisters();
    Map& getMap();
    ZoneCheckResult canEnterzone(Fighter* , Fighter* ,int );
  

   
    
};

#endif