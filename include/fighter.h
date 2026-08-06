#ifndef FIGHTER_H
#define FIGHTER_H
#include "zone.h"
#include "card.h"
#include "player.h"
#include <string>
#include <vector>

class Battle;
class Player;

enum Team
{
    SHERLOCK, DRACULA,INVISIBLE
};

struct AttackCardInfo
{
    int index;
    bool usable;
};
class Fighter
{
    std::string name;
    int health;
    int Maxhealth;
    bool ranged;
    int movement;
    
    Zone* position;
    std::vector<Card>deck;
    std::vector<Card>hand;
    Team team ;
    
    public:
    Fighter(std::string , int , int ,bool , int  , Team);

    vector<AttackCardInfo> getPlayableCardIndexes(Battle* , Fighter*  , Fighter*);

    virtual void setupUnits(Battle* , Player& );
   
    std::string getName() const;

    int getHealth() const;

    void takeDamage(int);

    bool isRanged() const;
    bool isalive()const;
    int getmovement()const;

    Zone* getPosition() const;

    void setPosition(Zone*);
    void heal (int );
    void addcard(Card);

    void showhand()const;
    int handsize() const;
    std::vector<Card>& gethand();
    Card playcard(int);

    Card remove_ranodmcard();

    bool isenemy (Fighter* );

    vector<Card> getrandomcard(int);
    void addtohand(const std:: vector<Card>&);
    int getdecksize()const;
    void setdeck(const std::vector<Card>& );
    void sethealth(int);
    int getMaxealth() const;
    Card drawBoostMovement();
    Team getteam ()const;
    virtual void specialAbillity(Battle*);
    
};

#endif