#include "fighter.h"
#include "card.h"
#include "battle.h"
#include<iostream>
#include<algorithm>
#include<random>
#include<cstdlib>
#include<ctime>



using namespace std;
Fighter::Fighter(string name,int health, int Maxhealth ,bool ranged , int  movement , bool heroteam )
{
    this->name = name;
    this->health =  health;
    this->Maxhealth =  Maxhealth;
    this->ranged = ranged;
    this->movement = movement;
    this->heroteam = heroteam;
    position = nullptr;
}

string Fighter::getName() const
{
    return name;
}

int Fighter::getHealth() const
{
    return health;
}
int Fighter:: getMaxealth() const
{
    return Maxhealth;
}

int Fighter::getmovement() const
{
    return movement;
}
void Fighter::takeDamage(int damage)
{
    health-=damage;
    if(health<0)
    health = 0;
}

bool Fighter::isRanged() const
{
    return ranged;
}
bool Fighter::isalive() const
{
    return health>0;
}

Zone* Fighter::getPosition() const
{
    return position;
}

void Fighter::setPosition(Zone* zone)
{
    position=zone;
}
void Fighter :: heal (int amount )
{
    health += amount;
    if(health> Maxhealth)
    health = Maxhealth;
                    
}
    void Fighter :: addcard(Card card)
    {
       deck.push_back(card);

    }
    
    void Fighter :: showhand()const
    {
        for(int i = 0 ; i <hand.size() ; i ++)
        {
             cout<<hand[i].getName()<<endl;
        }
    }

    int Fighter :: handsize() const
    {
            return hand.size();
    }

    vector<Card>&  Fighter :: gethand()
    {
        return hand;
    }


    Card Fighter :: playcard(int index)
    {
        Card selected = hand[index];
        hand.erase(hand.begin()+index);
        return selected;
    }


    Card  Fighter :: remove_ranodmcard()
    {
        int index = rand()% hand.size();
        Card temp = hand[index];

        hand.erase(hand.begin()+ index);

        return temp;
    }


    bool  Fighter:: isenemy (Fighter* other )
    {
        return ranged != other->ranged;

    }

    bool Fighter :: isheroteam() const
    {
        return heroteam;
    }

    vector<Card>  Fighter :: getrandomcard(int count )
    {
        vector<int>index;
        for(int i = 0 ; i <deck.size(); i++)
        {
            index.push_back(i);
        }
        
        if(count > deck.size())
        {
            count = deck.size();
        }
        if(deck.empty())
        {
            return {};
        }
        
        for ( int i = index.size()-1 ; i > 0 ; i--)
        {
           int j = rand()%(i+1);
           swap(index[i] ,index[j]);
        }
        
        
        vector<Card>rand;
         for(int i = 0 ; i < count ; i++)
         {
            rand.push_back(deck[index[i]]);
         }
         
         sort(index.begin() , index.begin()+ count ,greater<int>());
         
         for(int  i = 0 ; i < count ; i ++)
         deck.erase(deck.begin() + index[i]);
         
         return rand;
    }


    void Fighter :: addtohand(const std:: vector<Card>& cards)
    {
           for(const auto& c : cards)
           hand.push_back(c);
    }

    int  Fighter :: getdecksize()const
    {
        return deck.size();
    }
    
    Card Fighter :: removerandomcard()
    {
              int index = rand()% hand.size();
              Card temp = hand[index];
              hand.erase(hand.begin()+index);
              return temp;
    }

    void  Fighter :: setdeck(const vector<Card>& d)
    {
        deck = d;

    }
void  Fighter :: sethealth(int h)
{
    health = h;
}


vector<AttackCardInfo> Fighter::getPlayableCardIndexes(Battle* battle, Fighter* opponent , Fighter* actingfighter)
{
    
    vector<AttackCardInfo> result;

    for(int i = 0; i < hand.size(); i++)
    {
        Card& c = hand[i];
       
        bool usable = true;

        string owner = c.getfighterType();

        if( owner != "Any" && owner != actingfighter->getName())
        {
            result.push_back({i,false});
            continue;
        } 

        if(c.getcardType() == SCHEME)
        {
             usable = true ;
        }
        else if(c.getcardType()  == DEFENSE)
        {
            usable = false;
        }

        else if(c.getcardType() == ATTACK || c.getcardType() == VERSATILE )
        {

            if(actingfighter->getName() == "Dracula" || actingfighter->getName() == "Sister")
            {
                if(!battle->areadjacent(*actingfighter, *opponent))
                    usable = false;
    
            }
        }

        result.push_back({i, usable});
    }

    return result;
}



Card Fighter::drawBoostMovement()
{
    vector<Card> cards = getrandomcard(1);

    if(cards.empty())
        return Card();

    addtohand(cards);

    return hand.back();
}