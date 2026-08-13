#include "CombatManager.h"
#include "battle.h"
#include "cardeffect.h"
#include "handling.h"
#include <iostream>
#include <vector>

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

void CombatManager::resolveCombat(Fighter* attacker, Fighter* defender, Fighter* cardOwner, int attackCardIndex, int defenseCardIndex) {
    if(cardOwner->handsize() == 0) {
        cout << "Attacker has no cards\n";
        return;
    }
    if(defender->handsize() == 0 && defenseCardIndex != -1) {
        cout << "Defender has no cards\n";
        // ToDo: handle direct damage if needed
    }

    Card attackcard = cardOwner->playcard(attackCardIndex);
    currentAttackCard = &attackcard;
    this->lastAttackCard = attackcard;
    
    Card defendcard;
    currentDefendCard = nullptr;
    bool isdefended = false;

    // اگر کاربر دفاع کرده باشه (روی ضربدر قرمز کلیک نکرده باشه)
    if (defenseCardIndex != -1) {
        defendcard = defender->playcard(defenseCardIndex);
        currentDefendCard = &defendcard;
        isdefended = true;
    }

    // --- ادامه منطق دقیقا مثل کد خودت است ---
    ExecuteOrder order = getexecuteCardeffect(attackcard, defendcard, attacker, defender, isdefended);
    
    applycardeffect(*order.acard, order.aowner, order.atarget);
    
    if(order.bcard != nullptr) {
        if(!getCancel()) {
            applycardeffect(*order.bcard, order.bowner, order.btarget);
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

    cout << "Attacker played :" << attackcard.getName() << endl;
    if (isdefended) cout << "Defender played :" << defendcard.getName() << endl;
    
    int damage = attackValue - lastFinaldefend;

    if(damage > 0) {
        defender->takeDamage(damage);
         if(!defender->isalive()) {
            cout << defender->getName() << " died\n";
            if(defender->getName() == "Dracula" || defender->getName() == "Sherlock" || defender->getName() == "InvisibleMan") {
                battle->setGameOver(true);
                cout << "\n*** GAME OVER! " << defender->getName() << " has been defeated! ***\n";
                return;
            }
       }
        cout << "Damage taken ( " << damage << " )\n";
        cout << "Attacker won the combat!\n";
    } else {
        cout << "Damage taken ( 0 )\n";
        cout << "Defender won the combat!\n";   
    }
        
    this->setCancel(false);
}

void CombatManager::applycardeffect(Card& card, Fighter* attacker, Fighter* defender)
{
    CardEffect* effect = card.getEffect();
    if(effect)
    {
        effect->apply(attacker, defender, battle, card);
    }
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