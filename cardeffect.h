
#ifndef CARD_EFFECTS_H
#define CARD_EFFECTS_H

#include "card.h"
#include "fighter.h"
#include "battle.h"
#include "zone.h"
#include <vector>

class CardEffect {
public:
    virtual ~CardEffect() {}
    virtual void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card)=0;
 
};

class BloodThirstEffect : public CardEffect
{
public:
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card) override;
  
};
class AmbushEffect : public CardEffect
{
public:
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card) override;
  
};

class FeastEffect : public CardEffect
{
public:
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card) override;
  
};

#endif