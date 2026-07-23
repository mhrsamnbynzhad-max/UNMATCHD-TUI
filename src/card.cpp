#include "card.h"
#include "cardeffect.h"
#include"cardfactory.h"

vector<Cardinfo> Card::draculaCardDB = {
    { BEAST_FORM, "Beast Form", "Dracula", ATTACK, DURING_COMBAT, "Discard cards +1 attack", 6, 4, 2 },
    { FEAST, "Feast", "Dracula", SCHEME, IMMEDIATE, "Heal 2 and revive sister", 0, 2, 2 },
    { MIST_FORM, "Mist Form", "Dracula", SCHEME, IMMEDIATE, "Teleport and gain action", 0, 2, 2 },
    { HUNT, "Hunt", "Dracula", SCHEME, IMMEDIATE, "Damage adjacent enemies", 0, 4, 2 },
    { BLOOD_THIRST, "Blood Thirst", "Dracula", ATTACK, DURING_COMBAT, "Attack increases by sisters", 2, 3, 2 },
    { LOOK_INTO_MY_EYES, "Look Into My Eyes", "Dracula", DEFENSE, DURING_COMBAT, "Add opponent boost", 1, 2, 3 },
    { AMBUSH, "Ambush", "Any", ATTACK, DURING_COMBAT, "Discard random enemy card", 2, 3, 2 },
    { FEINT, "Feint", "Any", VERSATILE, DURING_COMBAT, "Cancel opponent effect", 2, 2, 3 },
    { MANEUVER, "Maneuver", "Any", VERSATILE, AFTER_COMBAT, "Move 3 spaces", 3, 1, 3 },
    { EXPLOIT, "Exploit", "Any", ATTACK, AFTER_COMBAT, "Draw card", 4, 1, 3 },
    { SURVIVAL_INSTINCT, "Survival Instinct", "Sister", ATTACK, AFTER_COMBAT, "Move Dracula near enemy", 3, 3, 3 },
    { SEDUCTIVE_CALL, "Seductive Call", "Sister", SCHEME, IMMEDIATE, "Move fighter and deal damage", 0, 2, 3 }
};

vector<Cardinfo> Card::sherlockCardDB = {
    { LEARNING_NEVER_ENDS,"Learning never ends","Any",VERSATILE,AFTER_COMBAT,"Draw Cards",3,1 , 2},
    { STRATEGIC_DEDUCTION,"Strategic Deduction","Sherlock",VERSATILE,DURING_COMBAT,"Use Boost Value",3,1,3},
    { COUNTER_ATTACK,"Counter attack","Sherlock",VERSATILE,AFTER_COMBAT,"Deal 2 Damage",3,1 , 3},
    { SIDEARM,"Sidearm","Watson",ATTACK,NONE,"None",5,3 , 2},
    { FIXED_POINT,"Fixed Point","Watson",VERSATILE,AFTER_COMBAT,"Heal Watson Holmes",3,1 ,2 },
    {SERVICE,"Service","Watson",SCHEME,IMMEDIATE,"Heal Holmes",0,2, 2 },//EMERGENCY AID
    { STUDY_METHOD,"Study method","Any",VERSATILE,AFTER_COMBAT,"See Enemy Hand",3,2, 2},
    { ELEMENTARY,"Elementary","Sherlock",DEFENSE,DURING_COMBAT,"Prediction",3,3 , 2},
    { IMPOSSIBLE,"Impossible","Sherlock",SCHEME,IMMEDIATE,"Burn Enemy Card",0,2 ,2 },//ELIMINTE IMPOSSIBLE
    { MASTER_OF_DISGUISE,"Master Of Disguise","Sherlock",SCHEME,IMMEDIATE,"Swap Positions",0,2,2 },
    { GAME_ON,"The Game Is Afoot","Sherlock",ATTACK,AFTER_COMBAT,"Move Holmes",5,2,2},//THE GAME IS ON
    { CONFIRM_SUSPICION,"Suspected Confirmed","Sherlock",SCHEME,IMMEDIATE,"Guess Card Value",0,1,3 },
    {DECEPTION,"Deception","Any",SCHEME,DURING_COMBAT,"Cancel all effects on your opponent's card",2,1,3}
};

Card Card::createFromInfo(const Cardinfo& info)
{
    static BloodThirstEffect bloodThirstEffect;
    static AmbushEffect ambushEffect;
    static FeastEffect feastEffect;
    static MonesterFormEffect beastFormEffect;
    static ExploitEffect exploitEffect;
    static HuntEyesEffect huntEyesEffect;
    static SeductivecallEffect seductivecallEffect;
    static SurvivalInstinctEffect survivalInstinctEffect;
    static FeintEffect feintEffect;
    static LookIntoMyEyesEffect lookIntoMyEyesEffect;
    static ManeuverEffect maneuverEffect;

    static CounterAttackEffect counterAttackEffect;
    static FixedPointEffect fixedPointEffect;
    static ServiceEffect serviceEffect;
    static StudyMethodEffect studyMethodEffect;
    static ElementaryEffect elementaryEffect;
    static ImpossibleEffect impossibleEffect;
    static MasterOfDisguiseEffect masterOfDisguiseEffect;
    static GameOnEffect gameOnEffect;
    static ConfirmSuspicionEffect confirmSuspicionEffect;
    static StrategicDeductionEffect strategicDeductionEffect;
    static LearningNeverEndsEffect learningNeverEndsEffect;
    static DeceptionEffect deceptionEffect;
    static SidearmEffect sidearmEffect;
    static MistFormEffect mistFormEffect;

     Card c(info.name, info.title, info.owner,info.type, info.timing, info.effect,info.value, info.boost);

    switch (info.name)
{
    // -------- Dracula --------

    case BLOOD_THIRST:
        c.effectroles =  &bloodThirstEffect;
        break;

    case AMBUSH:
        c.effectroles = &ambushEffect;
        break;

    case FEAST:
        c.effectroles = &feastEffect;
        break;

    case BEAST_FORM:
        c.effectroles = &beastFormEffect;
        break;

    case EXPLOIT:
        c.effectroles = &exploitEffect;
        break;

    case HUNT:
        c.effectroles = &huntEyesEffect;
        break;

    case SEDUCTIVE_CALL:
        c.effectroles = &seductivecallEffect;
        break;

    case SURVIVAL_INSTINCT:
        c.effectroles = &survivalInstinctEffect;
        break;


    case FEINT:
        c.effectroles = &feintEffect;
        break;


    case LOOK_INTO_MY_EYES:
        c.effectroles = &lookIntoMyEyesEffect;
        break;


    case MANEUVER:
        c.effectroles = &maneuverEffect;
        break;


    // -------- Sherlock / Watson --------


    case COUNTER_ATTACK:
        c.effectroles = &counterAttackEffect;
        break;


    case FIXED_POINT:
        c.effectroles = &fixedPointEffect;
        break;


    case SERVICE:
        c.effectroles = &serviceEffect;
        break;


    case STUDY_METHOD:
        c.effectroles = &studyMethodEffect;
        break;


    case ELEMENTARY:
        c.effectroles = &elementaryEffect;
        break;


    case IMPOSSIBLE:
        c.effectroles = &impossibleEffect;
        break;


    case MASTER_OF_DISGUISE:
        c.effectroles = &masterOfDisguiseEffect;
        break;


    case GAME_ON:
        c.effectroles = &gameOnEffect;
        break;


    case CONFIRM_SUSPICION:
        c.effectroles = &confirmSuspicionEffect;
        break;


    case STRATEGIC_DEDUCTION:
        c.effectroles = &strategicDeductionEffect;
        break;


    case LEARNING_NEVER_ENDS:
        c.effectroles = &learningNeverEndsEffect;
        break;


    case DECEPTION:
        c.effectroles = &deceptionEffect;
        break;


    case SIDEARM:
        c.effectroles = &sidearmEffect;
        break;



    // فعلا نداریم
    /*
    case MIST_FORM:
        c.effectroles = new MistFormEffect();
        break;
    */


    default:
        c.effectroles = nullptr;
        break;
}
    
     return c;
}

Card::Card(Cardname cn,string n, string f, Cardtype c,CardTime ti , string e, int v,int b)
{
    cardname = cn;
    name = n;
    fighterType = f;
    cardType = c;
    effect = e;
    timing = ti;
    value = v;
    boost = b;
    effectroles = nullptr;
   
}

string Card::getName() const
{
    return name;
}

    Cardname  Card:: getcardname() const
    {
        return cardname;
    }


string Card::getfighterType() const
{
    return fighterType;
}

Cardtype Card::getcardType() const
{
    return cardType;
}

int Card::getValue() const
{
    return value;
}

 void Card:: setValue(int v)
 {
     value = v ;
 }

CardEffect* Card:: getEffect()const
 {
    return effectroles;
 }

int Card::  getBoost()const
{
    return boost;
}
 void Card:: setBoost( int b)
 {
    boost = b ;
 }
  void Card:: increaseBoost(int amount)
  {
     boost += amount ;
  }

  CardTime Card::getTiming() const
    {
        return timing;
    }

     int  Card::  getPriority()const
     {
          switch ((timing))
          {
          case IMMEDIATE:
           return 0 ;
            break;

            case DURING_COMBAT:
           return 1 ;
            break;
          
            case AFTER_COMBAT:
           return 2 ;
            break;

          default:
          return 3;

            break;
          }
     }

     string Card :: cardtypetostring()const
    {
        switch(cardType)
        {
        case ATTACK: return "ATTACK";
        case DEFENSE: return "DEFENSE";
        case SCHEME: return "SCHEME";
        default: return "-";
        }
    }

    string Card :: timetostring()const
    {
        switch(timing)
        {
       
        case DURING_COMBAT: return "DURING";
        case AFTER_COMBAT: return "AFTER";
        case IMMEDIATE: return "IMMEDIATE";
        default: return "-";
        }
    }

     string Card :: getdescription()const
     {
        return effect;
     }

       string Card :: getowner()const
     {
        return fighterType;
     }

     CardTime Card :: gettime()const
     {
        return timing;
     }