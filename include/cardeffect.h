#ifndef CARD_EFFECTS_H
#define CARD_EFFECTS_H

#include "card.h"
#include "fighter.h"
#include "zone.h"
#include <vector>

class Battle;
class FogToken;
class CardEffect 
{
public:

    virtual ~CardEffect() {}
    virtual bool needsGUIInput() const { return false; }
    virtual bool needsMoreInput() const { return false; } 
    virtual bool usesHandSelection() const { return false; }
    virtual bool handSelectionRepeats() const { return false; }
    virtual bool allowsSkip() const { return handSelectionRepeats(); }
    virtual bool finishesOnSkip() const { return true; }
    virtual void onSkip(Fighter* attacker, Fighter* defender, Battle* battle) {}
    virtual void onHandSelectionStart(Fighter* attacker, Fighter* defender, Battle* battle) {}
    virtual bool usesNumberGuess() const { return false; }
    virtual int getNumberGuessMax() const { return 6; }
    virtual Fighter* getHandSelectionTarget(Fighter* attacker, Fighter* defender) const { return attacker; } 
    virtual std::vector<int> getValidZones(Fighter* attacker, Battle* battle) const 
    {
        return std::vector<int>();
    }

    virtual void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int guiChoice = -1) = 0;
 
};

class BloodThirstEffect : public CardEffect
{
public:
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int) override;
  
};
class AmbushEffect : public CardEffect
{
public:
   
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int) override;
  
};

class FeastEffect : public CardEffect
{
public:
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int) override;
  
};

class MonesterFormEffect : public CardEffect
{
public:
    bool needsGUIInput() const override;
    bool usesHandSelection() const override;
    bool handSelectionRepeats() const override;
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int) override;
};

class ManeuverEffect : public CardEffect
{
public:
    bool needsGUIInput() const override;
    std::vector<int> getValidZones(Fighter* attacker, Battle* battle) const override;
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int) override;
  
};
class ExploitEffect : public CardEffect
{
public:
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int) override;
  
};
class LookIntoMyEyesEffect : public CardEffect
{
public:
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int) override;
  
};

class  HuntEyesEffect : public CardEffect
{
public:
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int) override;
  
};

class SeductiveCallEffect : public CardEffect {
private:
    Fighter* selectedCard = nullptr; 
public:
    bool needsGUIInput() const override;
    bool needsMoreInput() const override;    
    std::vector<int> getValidZones(Fighter* , Battle* ) const override;
    void apply(Fighter* , Fighter* , Battle* , Card& , int guiChoice = -1) override;
};


class SurvivalInstinctEffect : public CardEffect {
private:
    Fighter* selectedOpponent = nullptr; 

public:
    bool needsGUIInput() const override;
    bool needsMoreInput() const override;   
    std::vector<int> getValidZones(Fighter* attacker, Battle* battle) const override;
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int guiChoice = -1) override;
};
class   FeintEffect : public CardEffect
{
public:
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int) override;
};

class CounterAttackEffect : public CardEffect
{
public:
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int) override;
};
    


class FixedPointEffect : public CardEffect
{
public:                                                                     
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int) override;
  
};


class ServiceEffect : public CardEffect {
private:
    Fighter* selectedOpponent = nullptr; 

public:
    bool needsGUIInput() const override;
    std::vector<int> getValidZones(Fighter* attacker, Battle* battle) const override;
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int guiChoice = -1) override;
};

class StudyMethodEffect : public CardEffect
{
public:
    bool needsGUIInput() const override;
    bool usesHandSelection() const override;
    Fighter* getHandSelectionTarget(
        Fighter* attacker,
        Fighter* defender) const override;

    void apply(
        Fighter* attacker,
        Fighter* defender,
        Battle* battle,
        Card& card,
        int guiChoice) override;
};


class ElementaryEffect : public CardEffect
{
public:
    bool needsGUIInput() const ;

    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int) override;
   
    
};


class ImpossibleEffect : public CardEffect
{
public:          
    bool needsGUIInput() const override;
    bool usesNumberGuess() const override;
    int getNumberGuessMax() const override;
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int) override;
};


class MasterOfDisguiseEffect : public CardEffect
{
public:                                                                        
    bool needsGUIInput() const ;

    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int) override;
    
};


class GameOnEffect : public CardEffect {
private:
 Fighter* selectedCard = nullptr; 

public:
    bool needsGUIInput() const override;
    std::vector<int> getValidZones(Fighter* , Battle* ) const override;
    void apply(Fighter* , Fighter* , Battle* , Card& , int guiChoice = -1) override;
};



class StrategicDeductionEffect : public CardEffect
{
public:
    bool needsGUIInput() const override;
    bool usesHandSelection() const override;
    Fighter* getHandSelectionTarget(
        Fighter* attacker,
        Fighter* defender
    ) const override;

    void apply(
        Fighter* attacker,
        Fighter* defender,
        Battle* battle,
        Card& card,
        int guiChoice
    ) override;
};

class ConfirmSuspicionEffect : public CardEffect
{
public:
    void apply(
        Fighter* attacker,
        Fighter* defender,
        Battle* battle,
        Card& card,
        int guiChoice
    ) override;
};

class LearningNeverEndsEffect : public CardEffect
{
public:                                                                

    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int) override;
    
};


class DeceptionEffect : public CardEffect
{
public:                                                                      
    bool needsGUIInput() const override;
    bool usesHandSelection() const override;
    Fighter* getHandSelectionTarget(Fighter* attacker, Fighter* defender) const override;
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int) override;
};


class SidearmEffect : public CardEffect
{
public:                                                                      

    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int) override;
};
class MistFormEffect : public CardEffect
{
public:                                                                      
  
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int) override;
};

class CodedNotesEffect : public CardEffect
{
private:
    int pickedCount = 0;
    Card firstPicked;
public:                                                                 
    bool needsGUIInput() const override;
    bool usesHandSelection() const override;
    bool handSelectionRepeats() const override;
    bool allowsSkip() const override;
    void onHandSelectionStart(Fighter* attacker, Fighter* defender, Battle* battle) override;
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int) override;
};

class ConfoundEffect : public CardEffect
{
private:
    enum class Stage { DISCARD_CHOICE, FOG_SELECT, FOG_DESTINATION };
    Stage stage = Stage::DISCARD_CHOICE;
    FogToken* selectedFog = nullptr;
public:                                                                       
    bool needsGUIInput() const override;
    bool usesHandSelection() const override;
    bool allowsSkip() const override;
    bool finishesOnSkip() const override;
    bool needsMoreInput() const override;
    Fighter* getHandSelectionTarget(Fighter* attacker, Fighter* defender) const override;
    std::vector<int> getValidZones(Fighter* attacker, Battle* battle) const override;
    void onSkip(Fighter* attacker, Fighter* defender, Battle* battle) override;
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int) override;
};

    class CovertPreparationEffect : public CardEffect {

        
        FogToken* selectedFog = nullptr;    
    public:

        void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int guiChoice = -1) override;
        bool needsMoreInput() const override;   
        bool needsGUIInput() const override;
        std::vector<int> getValidZones(Fighter* attacker, Battle* battle) const override;
    };

class DreamingOfRevengeEffect : public CardEffect
{
public:                                                                        

    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int) override;

};

class EmergefrommistEffect : public CardEffect
{
public:                                                                      

    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int) override;

};

class ImpossibletoseeEffect : public CardEffect
{
public:     
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int) override;

};

class IntoThinAirEffect : public CardEffect
{
public:
    bool needsGUIInput() const override;
    std::vector<int> getValidZones(Fighter* attacker, Battle* battle) const override;
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int) override;
  
};
class LurkingEffect : public CardEffect
{
public:
    bool needsGUIInput() const override;
    std::vector<int> getValidZones(Fighter* attacker, Battle* battle) const override;
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int) override;
  
};

class ReignOfTerrorEffect : public CardEffect
{
public:   
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int) override;

};

    class RollingFogEffect : public CardEffect {
    private:
        FogToken* selectedFog = nullptr;         
    public:
        bool needsGUIInput() const override;
        bool needsMoreInput() const override;     
        std::vector<int> getValidZones(Fighter* attacker, Battle* battle) const override;
        void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int) override;
    };

class SlipAwayEffect : public CardEffect
{
    private:
    int selectedFogIndex = -1;
     public:
        bool needsGUIInput() const override;
        bool needsMoreInput() const override;     
        std::vector<int> getValidZones(Fighter* attacker, Battle* battle) const override;
        void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int) override;
    };


class SteplightlyEffect : public CardEffect
{
private:
    FogToken* selectedFog = nullptr;
    bool damageDealt = false; 
public:
    bool needsGUIInput() const override;
    bool needsMoreInput() const override;
    std::vector<int> getValidZones(Fighter* attacker, Battle* battle) const override;
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int guiChoice = -1) override;
};

class VanishEffect : public CardEffect
{
public:      
    void apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int) override;

};


#endif