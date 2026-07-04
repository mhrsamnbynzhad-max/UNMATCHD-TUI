#include "fighter.h"
#include "card.h"
#include<iostream>
#include<algorithm>
#include<random>
#include<cstdlib>

using namespace std;
Fighter::Fighter(string name,int health,bool ranged , int  movement , bool heroteam)
{
    this->name = name;
    this->health =  health;
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
        std::shuffle (index.begin() , index.end(), std:: default_random_engine{});
        
        vector<Card>rand;
         for(int i = 0 ; i < count ; i++)
         {
            rand.push_back(deck[index[i]]);
         }
         
         sort(index.begin() , index.begin()+ count ,greater<int>());
         
         for(int  i = 0 ; i < count ; i ++)
         deck.erase(deck.begin() + index[i]);
         
         cout<<"++++++++++"<<deck.size()<<endl;

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






  

