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

    Card lastAttackCard;
    
    int lastFinaldefend= 0;

    bool cancelDEfendEffect = false;
    int finalAttackValue = 0;
    int finalDefendValue = 0;

    bool ignoreAttackValue = false;
    bool ignoreDefendValue = false;

    bool sherlockAbilityActive = false;

public:

    Battle();
    Player& getplayer1(){return player1;}
    Player& getplayer2(){return player2;}
    void setuppositions();
    void draculaability(Fighter* target);
    void printfighters();
    void combat(Fighter*,Fighter* , int);
    void applycardeffect(Card& , Fighter* ,Fighter* );
    bool areadjacent(Fighter& ,Fighter&  );
    bool movefighter(Fighter& ,int  , int );
    void showPossiblemoves(Fighter&);
    Fighter* getfighterat(Zone* );
    void showplacementzone(Fighter& );
    std::vector<Fighter>& getsisters();
    Map& getMap();
    ZoneCheckResult canEnterzone(Fighter* , Fighter* ,int );
    const Card& gelastattackcard()const{return lastAttackCard; }
    Fighter& getDracual(){return dracula;}
    Fighter& getSherlock(){return sherlock;}
    Fighter& getWatson(){return watson;}
    int getlastdefend()const{return lastFinaldefend;}
    void setCancel(int v){ cancelDEfendEffect = v;}
    bool getCancel()const{return cancelDEfendEffect ;}
    void setFinalAttackValue(int v){ finalAttackValue = v; }
    void setFinalDefendValue(int v){ finalDefendValue = v; }

    int getFinalAttackValue() const { return finalAttackValue; }
    int getFinalDefendValue() const { return finalDefendValue; }

    void setIgnoreAttack(bool v){ ignoreAttackValue = v; }
    void setIgnoreDefend(bool v){ ignoreDefendValue = v; }

    bool getIgnoreAttack() const { return ignoreAttackValue; }
    bool getIgnoreDefend() const { return ignoreDefendValue; }
    vector<Zone*> getReachableZone( Fighter& , int);
    void startTurn(Player& player);

    void useDraculaAbility();
    void useSherlockAbility();
};

#endif