#ifndef CARD_EFFECTS_H
#define CARD_EFFECTS_H

#include "card.h"
#include "fighter.h"
#include "zone.h"
#include <vector>

class Battle;
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

class MonesterFormEffect : public CardEffect
{
public:
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card) override;
  
};

class ManeuverEffect : public CardEffect
{
public:
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card) override;
  
};
class ExploitEffect : public CardEffect
{
public:
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card) override;
  
};
class LookIntoMyEyesEffect : public CardEffect
{
public:
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card) override;
  
};

class  HuntEyesEffect : public CardEffect
{
public:
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card) override;
  
};
class  SeductivecallEffect : public CardEffect
{
public:
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card) override;
  
};

class   SurvivalInstinctEffect : public CardEffect
{
public:
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card) override;
  
};
class   FeintEffect : public CardEffect
{
public:
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card) override;
  
};

class CounterAttackEffect : public CardEffect
{
public:
    void apply(Fighter* attacker,
               Fighter* defender,
               Battle* battle,
               Card& card) override;
};


class FixedPointEffect : public CardEffect
{
public:
    void apply(Fighter* attacker,
               Fighter* defender,
               Battle* battle,
               Card& card) override;
};


class ServiceEffect : public CardEffect
{
public:
    void apply(Fighter* attacker,
               Fighter* defender,
               Battle* battle,
               Card& card) override;
};


class StudyMethodEffect : public CardEffect
{
public:
    void apply(Fighter* attacker,
               Fighter* defender,
               Battle* battle,
               Card& card) override;
};


class ElementaryEffect : public CardEffect
{
public:
    void apply(Fighter* attacker,
               Fighter* defender,
               Battle* battle,
               Card& card) override;
};


class ImpossibleEffect : public CardEffect
{
public:
    void apply(Fighter* attacker,
               Fighter* defender,
               Battle* battle,
               Card& card) override;
};


class MasterOfDisguiseEffect : public CardEffect
{
public:
    void apply(Fighter* attacker,
               Fighter* defender,
               Battle* battle,
               Card& card) override;
};


class GameOnEffect : public CardEffect
{
public:
    void apply(Fighter* attacker,
               Fighter* defender,
               Battle* battle,
               Card& card) override;
};


class ConfirmSuspicionEffect : public CardEffect
{
public:
    void apply(Fighter* attacker,
               Fighter* defender,
               Battle* battle,
               Card& card) override;
};


class StrategicDeductionEffect : public CardEffect
{
public:
    void apply(Fighter* attacker,
               Fighter* defender,
               Battle* battle,
               Card& card) override;
};


class LearningNeverEndsEffect : public CardEffect
{
public:
    void apply(Fighter* attacker,
               Fighter* defender,
               Battle* battle,
               Card& card) override;
};


class DeceptionEffect : public CardEffect
{
public:
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card) override;
};


class SidearmEffect : public CardEffect
{
public:
    void apply(Fighter* attacker, Fighter* defender,  Battle* battle,   Card& card) override;
};
class MistFormEffect : public CardEffect
{
public:

    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card) override;
};

class CodedNotesEffect : public CardEffect
{
public:
    void apply(Fighter* attacker,Fighter* defender,  Battle* battle,  Card& card) override;
};

class ConfoundEffect : public CardEffect
{
public:
    void apply(Fighter* attacker,  Fighter* defender,Battle* battle, Card& card) override;
};

class CovertPreparationEffect : public CardEffect
{
public:
    void apply(Fighter* attacker,  Fighter* defender,Battle* battle, Card& card) override;
};

class DreamingOfRevengeEffect : public CardEffect
{
public:

    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card) override;

};

class EmergefrommistEffect : public CardEffect
{
public:

    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card) override;

};

class ImpossibletoseeEffect : public CardEffect
{
public:

    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card) override;

};

class IntoThinAirEffect : public CardEffect
{
public:

    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card) override;

};

class LurkingEffect : public CardEffect
{
public:

    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card) override;

};

class ReignOfTerrorEffect : public CardEffect
{
public:

    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card) override;

};

class RollingFogEffect : public CardEffect
{
public:

    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card) override;

};

class SlipAwayEffect : public CardEffect
{
public:

    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card) override;

};

class SteplightlyEffect : public CardEffect
{
public:

    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card) override;

};

class VanishEffect : public CardEffect
{
public:

    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card) override;

};

#endif