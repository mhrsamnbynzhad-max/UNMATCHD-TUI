#include "CombatManager.h"
#include "battle.h"
#include "cardeffect.h"
#include "handling.h"
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

  vector<int> CombatManager::getValidDefenseCards(Fighter* defender) {
    vector<int> defenseIndexes;
    if (defender == nullptr || defender->handsize() == 0) return defenseIndexes;
    
    for(int i = 0; i < defender->handsize(); i++) {
        Cardtype type = defender->gethand()[i].getcardType();
        if(type == DEFENSE || type == VERSATILE) {
            defenseIndexes.push_back(i);
        }
    }
    return defenseIndexes;
}

void CombatManager::applycardeffect(Card& card, Fighter* attacker, Fighter* defender, int guiChoice)
{
    CardEffect* effect = card.getEffect();
    if(effect) effect->apply(attacker, defender, battle, card, guiChoice);
}

void CombatManager::resolveCombat(Fighter* attacker, Fighter* defender, Fighter* cardOwner, int attackCardIndex, int defenseCardIndex , int defenseGuiChoice ) {
    if(cardOwner->handsize() == 0) {
        cout << "Attacker has no cards\n";
        return;
    }
    if(defender->handsize() == 0 && defenseCardIndex != -1) {
        cout << "Defender has no cards\n";
    }

    combatAttacker = attacker;
    combatDefender = defender;

    Card attackcard = cardOwner->playcard(attackCardIndex);
    currentAttackCard = &attackcard;
    this->lastAttackCard = attackcard;
    
    Card defendcard;
    currentDefendCard = nullptr;
    bool isdefended = false;

    if (defenseCardIndex != -1) {
        defendcard = defender->playcard(defenseCardIndex);
        currentDefendCard = &defendcard;
        isdefended = true;
    }

    ExecuteOrder order = getexecuteCardeffect(attackcard, defendcard, attacker, defender, isdefended);

    bool aBeforeDamage = order.acard->getPriority() < 2;
    bool bBeforeDamage = (order.bcard != nullptr) && order.bcard->getPriority() < 2;

    if (aBeforeDamage) {
        applycardeffect(*order.acard, order.aowner, order.atarget,
                         (order.aowner == defender ? defenseGuiChoice : -1));
    }
    if (bBeforeDamage) {
        if(!getCancel()) {
            applycardeffect(*order.bcard, order.bowner, order.btarget,
                             (order.bowner == defender ? defenseGuiChoice : -1));
        } else {
            cout << "Opponent card effect was cancelled\n";
        }
    }
        
   int attackValue = attackcard.getValue();
    int defederValue = isdefended ? defendcard.getValue() : 0;
    
    if(battle->getinvisibleactive() && (defendcard.getcardType() == DEFENSE || defendcard.getcardType() == VERSATILE)) {
        lastFinaldefend = defederValue + 1;
    } else {
        lastFinaldefend = defederValue;
    }

    setFinalAttackValue(attackValue);
    setFinalDefendValue(lastFinaldefend);

    ostringstream dmgMsg;
   cout << "Attacker played :" << attackcard.getName() << endl;
    if (isdefended) {
        cout << "Defender played :" << defendcard.getName() << endl;
    }
    
    int damage = attackValue - lastFinaldefend;

    if(damage > 0) {
        defender->takeDamage(damage);
         if(!defender->isalive()) {
            cout << defender->getName() << " died\n";
            dmgMsg << defender->getName() << " died\n";
            if(defender->getName() == "Dracula" || defender->getName() == "Sherlock" || defender->getName() == "InvisibleMan") {
                battle->setGameOver(true);
                cout << "\n*** GAME OVER! " << defender->getName() << " has been defeated! ***\n";
                dmgMsg << "*** GAME OVER! " << defender->getName() << " has been defeated! ***";
                lastCombatMessage = dmgMsg.str();
                return;
            }
       }
        cout << "Damage taken ( " << damage << " )\n";
        cout << "Attacker won the combat!\n";
        dmgMsg << "Damage  taken  (  " << damage << "  )\n";
        dmgMsg << "Attacker   won   the   combat!";
    } else {
        cout << "Damage  taken  (  0  )\n";
        cout << "Defender won the combat!\n";   
        dmgMsg << "Damage   taken   (  0  )\n";
        dmgMsg << "Defender   won   the   combat!";
    }

   if (!aBeforeDamage) {
        applycardeffect(*order.acard, order.aowner, order.atarget,
                         (order.aowner == defender ? defenseGuiChoice : -1));
        if (order.acard->getEffect() != nullptr && order.acard->getEffect()->needsPostCombatGUI()) {
            pendingPostCombatEffect = order.acard->getEffect();
            pendingPostCombatAttacker = order.aowner;
            pendingPostCombatDefender = order.atarget;
        }
    }
    if (order.bcard != nullptr && !bBeforeDamage) {
        if(!getCancel()) {
            applycardeffect(*order.bcard, order.bowner, order.btarget,
                             (order.bowner == defender ? defenseGuiChoice : -1));
            if (order.bcard->getEffect() != nullptr && order.bcard->getEffect()->needsPostCombatGUI()) {
                pendingPostCombatEffect = order.bcard->getEffect();
                pendingPostCombatAttacker = order.bowner;
                pendingPostCombatDefender = order.btarget;
            }
        } else {
            cout << "Opponent card effect was cancelled\n";
        }
    }

    lastCombatMessage = dmgMsg.str();
    this->setCancel(false);
}

ExecuteOrder CombatManager::getexecuteCardeffect(Card& attackCard, Card& defendCard, Fighter* attacker, Fighter* defender, bool defended)
{
    ExecuteOrder order;
    if(!defended)
    {
        order.acard = &attackCard;
        order.aowner = attacker;
        order.atarget = defender;
        order.bcard = nullptr;
        order.bowner = nullptr;
        order.btarget = nullptr;
        return order;
    }

    if(attackCard.getPriority() < defendCard.getPriority())
    {
        order.acard = &attackCard;
        order.aowner = attacker;
        order.atarget = defender;
        order.bcard = &defendCard;
        order.bowner = defender;
        order.btarget = attacker;
    }
    else
    {
        order.acard = &defendCard;
        order.aowner = defender;
        order.atarget = attacker;
        order.bcard = &attackCard;
        order.bowner = attacker;
        order.btarget = defender;
    }
    return order;
}

void CombatManager::combat(Fighter* attacker, Fighter* defender, Fighter* cardOwner,int cardindex) {
    int attackCardIndex = 0; 
    int defenseCardIndex = -1;
    resolveCombat(attacker, defender, cardOwner, attackCardIndex, defenseCardIndex);
}