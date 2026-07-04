#ifndef CARD_H
#define CARD_H
#include <string>
#include <vector>
class CardEffect;

using namespace std;
enum Cardtype {ATTACK, DEFENSE , VERSATILE , SCHEME};
enum Cardname {BEAST_FORM, FEAST , MIST_FORM ,DECEPTION, HUNT , BLOOD_THIRST , LOOK_INTO_MY_EYES , AMBUSH , FEINT , MANEUVER , EXPLOIT , SURVIVAL_INSTINCT , SEDUCTIVE_CALL ,GAME_ON,MASTER_OF_DISGUISE,IMPOSSIBLE,ELEMENTARY,STUDY_METHOD,SERVICE,FIXED_POINT,SIDEARM,COUNTER_ATTACK,STRATEGIC_DEDUCTION,LEARNING_NEVER_ENDS, CONFIRM_SUSPICION};
enum CardTime {BEFOR_COMBAT, DURING_COMBAT , AFTER_COMBAT , IMMEDIATE , NONE};

struct Cardinfo {
    Cardname name;
    string title;
    string owner;
    Cardtype type;
    CardTime timing;
    string effect;
    int value;
    int boost;
    int count;
};



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

    CardEffect* effectroles = nullptr;

    
    
public:    
    static vector<Cardinfo> draculaCardDB;
    static vector<Cardinfo> sherlockCardDB;
    static Card createFromInfo(const Cardinfo& );
    Card() : name("NO_CARD"), value(0), cardType(DEFENSE)
    {
    }
    Card(Cardname,string ,string,Cardtype, CardTime, string ,int,int);

    string getName() const;
    string getfighterType() const;
    Cardtype getcardType() const;
    int getValue() const;
    Cardname getcardname() const;
    void setValue(int);
    int getBoost()const;
    void setBoost( int );
    void increaseBoost(int);
    CardEffect* getEffect()const;
    
};
#endif