#ifndef CARD_H
#define CARD_H

#include <string>

using namespace std;
enum Cardtype {ATTACK, DEFENSE , VERSATILE , SCHEME};
enum Cardname {BEAST_FORM, FEAST , MIST_FORM ,DECEPTION, HUNT , BLOOD_THIRST , LOOK_INTO_MY_EYES , AMBUSH , FEINT , MANEUVER , EXPLOIT , SURVIVAL_INSTINCT , SEDUCTIVE_CALL ,GAME_ON,MASTER_OF_DISGUISE,IMPOSSIBLE,ELEMENTARY,STUDY_METHOD,SERVICE,FIXED_POINT,SIDEARM,COUNTER_ATTACK,STRATEGIC_DEDUCTION,LEARNING_NEVER_ENDS, CONFIRM_SUSPICION};
enum CardTime {BEFOR_COMBAT, DURING_COMBAT , AFTER_COMBAT , IMMEDIATE , NONE};


class Card
{
private:

    Cardname  cardname;
    string name;

    string fighterType;
    
    Cardtype cardType;

    string effect;
    CardTime timing;

    int value;

    int boost;
    
    
public:    

    Card(Cardname,string ,string,Cardtype, CardTime, string ,int,int);
     
    string getName() const;

    string getfighterType() const;

    Cardtype getcardType() const;

    int getValue() const;

    int getBoost() const;
    Cardname getcardname() const;
};


#endif