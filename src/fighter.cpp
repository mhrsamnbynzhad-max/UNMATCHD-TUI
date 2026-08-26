#include "fighter.h"
#include "card.h"
#include "handling.h"
#include "battle.h"
#include "player.h"
#include "boardmanager.h"
#include "combatmanager.h"

#include<iostream>
#include<algorithm>
#include<random>
#include<cstdlib>
#include<ctime>



using namespace std;
Fighter::Fighter(string name,int health, int Maxhealth ,bool ranged , int  movement , Team team )
{
    this->name = name;
    this->health =  health;
    this->Maxhealth =  Maxhealth;
    this->ranged = ranged;
    this->movement = movement;
    this->team = team ;
    position = nullptr;
}

string Fighter::getName() const
{
    return name;
}
Team  Fighter:: getteam ()const
{
    return team;
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
    if( health == 0)
    {
        position = nullptr;
    }
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
        if(!validIndex(index,hand.size()))
        {
            cout<<"Invalid card index";
            return Card();
        }
        Card selected = hand[index];
        hand.erase(hand.begin()+index);
        return selected;
    }


    Card  Fighter :: remove_ranodmcard()
    {
        if(hand.empty())
        {
            cout<<" Hand is empty\n";
            return Card();
        }
        int index = rand()% hand.size();
        Card temp = hand[index];

        hand.erase(hand.begin()+ index);

        return temp;
    }


    bool  Fighter:: isenemy (Fighter* other )
    {
        return ranged != other->ranged;

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

    void  Fighter :: setdeck(const vector<Card>& d)
    {
        deck = d;

    }
void  Fighter :: sethealth(int h)
{
    health = h;
}
   vector<AttackCardInfo> Fighter::getPlayableCardIndexes(Battle* battle, Fighter* opponent, Fighter* actingfighter)
{
    vector<AttackCardInfo> result;

    vector<Fighter*> aliveEnemies;
    for (Fighter* f : battle->getFighters())
    {
        if (f != nullptr && f->isalive() && f->getteam() != actingfighter->getteam())
        {
            aliveEnemies.push_back(f);
        }
    }

    for (int i = 0; i < hand.size(); i++)
    {
        Card& c = hand[i];
        bool usable = true;
        string owner = c.getfighterType();

        bool ownerAlive = false;
        if (owner == "Any")
        {
            ownerAlive = true;
        } 
        else if (owner == actingfighter->getName())
        {
            ownerAlive = actingfighter->isalive();
        } 
        
        else if (owner == "Sister") {
            for (auto& s : battle->getsisters())
            {
                if (s.isalive()) { ownerAlive = true; break; }
            }
        } 
        else if (owner == "Watson")
        {
            ownerAlive = battle->getWatson().isalive(); 
        } 
        else 
        {
            ownerAlive = true; 
        }

        if (!ownerAlive)
        {
            result.push_back({i, false});
            continue;
        }

        if (c.getcardType() == SCHEME)
        {
            usable = true;
        }
        else if (c.getcardType() == DEFENSE)
        {
            usable = false;
        }
        else if (c.getcardType() == ATTACK || c.getcardType() == VERSATILE)
        {
            usable = false; 
            if (owner == "Dracula")
            {
                for (Fighter* enemy : aliveEnemies) {
                    if (battle->getBoard()->areadjacent(battle->getDracual(), *enemy)) {
                        usable = true; break;
                    }
                }
            }
            else if (owner == "Sister")
            {
                for (auto& s : battle->getsisters()) {
                    if (!s.isalive()) continue;
                    for (Fighter* enemy : aliveEnemies) {
                        if (battle->getBoard()->areadjacent(s, *enemy)) {
                            usable = true; break; 
                        }
                    }
                    if(usable) break;
                }
            }
            else if (owner == "Sherlock" || owner == "Watson")
            {
                Fighter& shooter = (owner == "Sherlock") ? battle->getSherlock() : battle->getWatson();
                vector<Zone*> zones = battle->getMap().getplacementZone(shooter.getPosition());

                for (Fighter* enemy : aliveEnemies) {
                    for (auto z : zones) {
                        if (z == enemy->getPosition()) {
                            usable = true; break;
                        }
                    }
                    if(usable) break;
                }
            }
            else if (owner == "Any")
            {
                for (Fighter* enemy : aliveEnemies) {
                    if (battle->getBoard()->areadjacent(battle->getDracual(), *enemy)) {
                        usable = true; break;
                    }
                }
                if (!usable) {
                    for (auto& s : battle->getsisters()) {
                        if (!s.isalive()) continue;
                        for (Fighter* enemy : aliveEnemies) {if (battle->getBoard()->areadjacent(s, *enemy)) {
                                usable = true; break;
                            }
                        }
                        if(usable) break;
                    }
                }
            }
            else
            {
                if (actingfighter->isRanged())
                {
                    vector<Zone*> zones = battle->getMap().getplacementZone(actingfighter->getPosition());
                    for (Fighter* enemy : aliveEnemies) {
                        for (auto z : zones) {
                            if (z == enemy->getPosition()) {
                                usable = true; break;
                            }
                        }
                        if(usable) break;
                    }
                }
                else
                {
                    for (Fighter* enemy : aliveEnemies) {
                        if (battle->getBoard()->areadjacent(*actingfighter, *enemy)) {
                            usable = true; break;
                        }
                    }
                }
            }
        }

        result.push_back({i, usable});
    }

    return result;
}

Card Fighter::drawBoostMovement()
{
    Card card = drawTopCard();

    if(card.getName() == "")
        return Card();

    addtohand({card});

    return hand.back();
}


 void  Fighter:: specialAbillity(Battle* battle)
{
}

void Fighter::setupUnits(Battle* battle, Player& player)
{
}

Card Fighter::drawTopCard()
{
    if(deck.empty())
        return Card();

    Card c = deck.back();
    deck.pop_back();
    return c;
}

void Fighter::putCardOnTop(const Card& card)
{
    deck.push_back(card);
}


