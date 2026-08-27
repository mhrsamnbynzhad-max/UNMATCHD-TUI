#ifndef COMBATMANAGER_H
#define COMBATMANAGER_H

#include "GameTypes.h"
#include "card.h"
#include <string>

class Battle;
class Fighter;
class CardEffect;

class CombatManager {
private:
    Battle* battle; 
    Card* currentAttackCard = nullptr;
    Card* currentDefendCard = nullptr;
    Card lastAttackCard;
    
   int lastFinaldefend = 0;
    std::string lastCombatMessage;
    bool cancelDEfendEffect = false;
    int finalAttackValue = 0;
    int finalDefendValue = 0;
    bool ignoreAttackValue = false;
    bool ignoreDefendValue = false;
    CardEffect* pendingPostCombatEffect = nullptr;
    Fighter* pendingPostCombatAttacker = nullptr;
    Fighter* pendingPostCombatDefender = nullptr;
    Fighter* combatAttacker = nullptr;
    Fighter* combatDefender = nullptr;

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
    const std::string& getLastCombatMessage() const { return lastCombatMessage; }
    CardEffect* getPendingPostCombatEffect() const { return pendingPostCombatEffect; }
    Fighter* getPendingPostCombatAttacker() const { return pendingPostCombatAttacker; }
    Fighter* getPendingPostCombatDefender() const { return pendingPostCombatDefender; }
    Fighter* getCombatAttacker() const { return combatAttacker; }
    Fighter* getCombatDefender() const { return combatDefender; }
    void clearPendingPostCombat() { pendingPostCombatEffect = nullptr; pendingPostCombatAttacker = nullptr; pendingPostCombatDefender = nullptr; }    void resolveCombat(Fighter* attacker, Fighter* defender, Fighter* cardOwner, int attackCardIndex, int defenseCardIndex, int defenseGuiChoice = -1);
    void applycardeffect(Card& card, Fighter* attacker, Fighter* defender, int guiChoice = -1);

};

#endif