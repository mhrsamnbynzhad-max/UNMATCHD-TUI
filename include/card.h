#ifndef CARD_H
#define CARD_H
#include <string>
#include <vector>
class CardEffect;

using namespace std;
enum Cardtype {ATTACK, DEFENSE , VERSATILE , SCHEME};
enum Cardname {BEAST_FORM, FEAST , MIST_FORM ,DECEPTION, HUNT , BLOOD_THIRST , LOOK_INTO_MY_EYES , AMBUSH , FEINT , MANEUVER ,
     EXPLOIT , SURVIVAL_INSTINCT , SEDUCTIVE_CALL ,GAME_ON,MASTER_OF_DISGUISE,IMPOSSIBLE,ELEMENTARY,STUDY_METHOD,SERVICE,FIXED_POINT,
     SIDEARM,COUNTER_ATTACK,STRATEGIC_DEDUCTION,LEARNING_NEVER_ENDS, CONFIRM_SUSPICION ,
     COVERT_PREPARATION , CONFOUND , CODED_NOTES , DREAMING_OF_REVENGE , EMERGE_FROM_MIST , IMPOSSIBLE_TO_SEE  ,  INTO_THIN_AIR , LURKING ,REIGN_OF_TERROR,
    ROLLING_FOG , SLIP_AWAY , STEP_LIGHTLY , VANISH ,};
enum CardTime {DURING_COMBAT , AFTER_COMBAT , IMMEDIATE , NONE};

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
    bool valueLocked = false;

    
    
public:   
    
    static vector<Cardinfo> draculaCardDB;
    static vector<Cardinfo> sherlockCardDB;
    static vector<Cardinfo> invisiblemanCardDB;
    static Card createFromInfo(const Cardinfo& );
    Card() : name("NO_CARD"), value(0), cardType(DEFENSE)
    {
    }
    Card(Cardname,string ,string,Cardtype, CardTime, string ,int,int);

    string getName() const;
    string getfighterType() const;
    Cardtype getcardType() const;
    int getValue() const;
    void setValue(int);
     void lockValue();
     bool isValueLocked() const;
     void forceValue(int );
    Cardname getcardname() const;
    int getBoost()const;
    void setBoost( int );
    void increaseBoost(int);
    CardEffect* getEffect()const;
    CardTime getTiming() const;
    int getPriority()const;

    string getdescription()const;
    string getowner()const;
    CardTime gettime()const;

    string cardtypetostring()const;
    string timetostring()const;

    
};
#endif