#ifndef BATTLE_H
#define BATTLE_H

#include "Player.h"
#include "fighter.h"
#include "dracula.h"
#include "watson.h"
#include "sisters.h"
#include "sherlock.h"
#include "invisible.h"
#include "fogtoken.h"
#include "map.h"
#include <vector>
#include <queue>
#include "GameTypes.h"

// Forward declarations
class CombatManager;
class BoardManager;
class Card;

class Battle
{
private:
    Sherlock sherlock;
    Watson watson;
    Dracula dracula;
    InvisibleMan invisibleman;

    std::vector<Sisters> sisters;
    std::vector<FogToken> fogtoken;

    Map map;
    Player player1;
    Player player2;

    CombatManager* combatManager;
    BoardManager* boardManager;

    bool sherlockAbilityActive = false;
    bool invisibleAbilityActive = false;
    bool extraAction = false;
    bool playerfirst = true;
    bool gameover = false;
    bool invisibleStartedOnFog = false;

    std::vector<Fighter*> availableHeroes;
    int sidekickIndex = 0;
    int fogIndex = 0;
    std::queue<Player*> turnQueue;
    int actionsThisTurn = 0;
    int guiChoice = -1;

public:
    Battle();
    ~Battle();


    CombatManager* getCombat() { return combatManager; }
    BoardManager* getBoard() { return boardManager; }

  
    Player& getplayer1() { return player1; }
    Player& getplayer2() { return player2; }
    Map& getMap() { return map; }
    std::vector<FogToken>& getfogtoken() { return fogtoken; }
    std::vector<Sisters>& getsisters();

    Fighter& getDracual() { return dracula; }
    Dracula& getDraculaObj() { return dracula; }
    Fighter& getSherlock() { return sherlock; }
    Fighter& getWatson() { return watson; }
    InvisibleMan& getInvisibleMan() { return invisibleman; }

    Fighter* getfighterat(Zone* zone);
    Player* getPlayerOfFighter(Fighter* fighter);
    std::vector<Fighter*> getAllFighters();
    std::vector<Fighter*> getFighters();

    void setuppositions();
    void chooseHeroes(Player&, Player&);
    void chooseSidekickPosition(Player&);
    void chooseFogPosition(Player&);
    void startTurn(Player& player);
    void printfighters();
    void draculaability(Fighter* target);
    void checkInvisibleFogAtTurnStart();

    void setGameOver(bool val) { gameover = val; }
    bool isgameover() const { return gameover; }
    bool getplayerfirst() const { return playerfirst; }
    
    void giveExtraAction() { extraAction = true; }
    bool hasExtraAction() { return extraAction; }
    void resetExtraAction() { extraAction = false; }
    void setSherlockability(bool value) { sherlockAbilityActive = value; }
    void setinvisibleability(bool value) { invisibleAbilityActive = value; }
    bool getinvisibleactive() const { return invisibleAbilityActive; }
    bool startedTurnOnFog() const { return invisibleStartedOnFog; }
    void setAges(int , int );         
    Player& getFirstChooser();
    Player& getSecondChooser();
    std::vector<Fighter*>& getAvailableHeroes();
    void assignHero(Player& , Fighter* );
    void finalizeSetup(); 
    std::vector<int> getSidekickValidZones(Player& player);
    bool placeSidekickAt(Player& player, int zoneId); 
    void beginUnitSetup(); 
    void startGame();
    Player& getCurrentPlayer();
    Player& getOtherPlayer(Player& );
    void addAction() { actionsThisTurn++; }
    void resetActionCounter() { actionsThisTurn = 0; }
    bool turnShouldEnd() const { return actionsThisTurn >= 2; }
    void endTurnAndAdvance();
    void setGUIChoice(int choice);
    bool hasGUIChoice() const;
    int getGUIChoice();
};

#endif