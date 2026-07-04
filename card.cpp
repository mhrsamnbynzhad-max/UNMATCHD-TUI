#include "card.h"
#include "cardeffect.h"


vector<Cardinfo> Card::draculaCardDB = {
    { BEAST_FORM, "Beast Form", "Dracula", ATTACK, DURING_COMBAT, "Discard cards +1 attack", 6, 4, 2 },
    { FEAST, "Feast", "Dracula", SCHEME, IMMEDIATE, "Heal 2 and revive sister", 0, 2, 2 },
    { MIST_FORM, "Mist Form", "Dracula", SCHEME, IMMEDIATE, "Teleport and gain action", 0, 2, 2 },
    { HUNT, "Hunt", "Dracula", SCHEME, IMMEDIATE, "Damage adjacent enemies", 0, 4, 2 },
    { BLOOD_THIRST, "Blood Thirst", "Dracula", ATTACK, DURING_COMBAT, "Attack increases by sisters", 2, 3, 2 },
    { LOOK_INTO_MY_EYES, "Look Into My Eyes", "Dracula", DEFENSE, DURING_COMBAT, "Add opponent boost", 1, 2, 3 },
    { AMBUSH, "Ambush", "Any", ATTACK, DURING_COMBAT, "Discard random enemy card", 2, 3, 2 },
    { FEINT, "Feint", "Any", VERSATILE, BEFOR_COMBAT, "Cancel opponent effect", 2, 2, 3 },
    { MANEUVER, "Maneuver", "Any", VERSATILE, AFTER_COMBAT, "Move 3 spaces", 3, 1, 3 },
    { EXPLOIT, "Exploit", "Any", VERSATILE, AFTER_COMBAT, "Draw card", 4, 1, 3 },
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
    {DECEPTION,"Deception","Any",SCHEME,BEFOR_COMBAT,"Cancel all effects on your opponent's card",2,1,3}
};

Card Card::createFromInfo(const Cardinfo& info)
{
     Card c(info.name, info.title, info.owner,info.type, info.timing, info.effect,info.value, info.boost);


     switch (info.name)
     {
     case BLOOD_THIRST:
        c.effectroles = new BloodThirstEffect(); 
        break;

    case AMBUSH:
        c.effectroles = new AmbushEffect(); 
        break;

    case FEAST:
        c.effectroles = new FeastEffect(); 
        break;
     
     default:
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
