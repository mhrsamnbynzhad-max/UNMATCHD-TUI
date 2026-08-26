#ifndef COMBATMANAGER_H
#define COMBATMANAGER_H

#include "gametypes.h"
#include "card.h"

class Battle;
class Fighter;

class CombatManager {
private:
    Battle* battle; 
    Card* currentAttackCard = nullptr;
    Card* currentDefendCard = nullptr;
    Card lastAttackCard;
    
    int lastFinaldefend = 0;
    bool cancelDEfendEffect = false;
    int finalAttackValue = 0;
    int finalDefendValue = 0;
    bool ignoreAttackValue = false;
    bool ignoreDefendValue = false;

    ExecuteOrder getexecuteCardeffect(Card&, Card&, Fighter*, Fighter*, bool);

public:
    CombatManager(Battle* b) : battle(b) {}
    void combat(Fighter* attacker, Fighter* defender, Fighter* cardOwner, int cardindex);
    std::vector<int> getValidDefenseCards(Fighter* defender);

    // Getters & Setters
    void setCancel(int v) { cancelDEfendEffect = v; }
    bool getCancel() const { return cancelDEfendEffect; }
    void setFinalAttackValue(int v) { finalAttackValue = v; }
    void setFinalDefendValue(int v) { finalDefendValue = v; }
    int getFinalAttackValue() const { return finalAttackValue; }
    int getFinalDefendValue() const { return finalDefendValue; }
    void setIgnoreAttack(bool v) { ignoreAttackValue = v; }
    void setIgnoreDefend(bool v) { ignoreDefendValue = v; }
    bool getIgnoreAttack() const { return ignoreAttackValue; }
    bool getIgnoreDefend() const { return ignoreDefendValue; }
    int getlastdefend() const { return lastFinaldefend; }
    Card* getCurrentAttackCard() { return currentAttackCard; }
    Card* getCurrentDefendCard() { return currentDefendCard; }
    const Card& gelastattackcard() const { return lastAttackCard; }
    void resolveCombat(Fighter* attacker, Fighter* defender, Fighter* cardOwner, int attackCardIndex, int defenseCardIndex, int defenseGuiChoice = -1);
    void applycardeffect(Card& card, Fighter* attacker, Fighter* defender, int guiChoice = -1);
};

#endif