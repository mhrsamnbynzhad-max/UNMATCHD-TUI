#include "player.h"
#include "battle.h"
#include "handling.h"
#include <iostream>


using namespace std;

Player::Player(const string& n) : name(n), hero(nullptr)
{
}

string Player::getName() const
{
    return name;
}

Fighter* Player::getHero() const
{
    return hero;
}

void Player::chooseHero(Fighter* sherlock, Fighter* dracula)
{
    int choice;

    while (true)
    {

        cout << "1) Sherlock\n";
        cout << "2) Dracula\n";

        choice = readInt( "\n" + name +" Choose your FIGHTER:" , 1 ,2 );

        if (choice == 1)
        {
            hero = sherlock;
            break;
        }
        else if (choice == 2)
        {
            hero = dracula;
            break;
        }
        else
        {
            cout << "Invalid selection, must be 1 or 2.\n";
        }
    }

    cout << name << " selected " << hero->getName() << "!\n";
}
void Player::drawCard()
{
    if(hero == nullptr)
        throw logic_error("The player HAS NOT SELECTED a fighter yet..!");

        auto cards = hero->getrandomcard(5);

        hero->addtohand(cards);
  
    
}

void Player::maneuver(Battle& battle)
{
    if(hero == nullptr)
        throw logic_error("The player HAS NOT SELECTED a fighter yet..!");

        Card drawn = hero->drawBoostMovement();

        cout << "\nA card was drawn and added to your hand.\n";

        vector<Card>& hand = hero->gethand();

        for(int i=0;i<hand.size();i++)
        {
            cout << i+1 << ") " << hand[i].getName() << " (Boost = " << hand[i].getBoost() << ")\n";
        }

        int boost = 0;
        int choice;
        choice = readInt("Choose a card to use its Boost (0 = No Boost): " , 0 , hand.size()) ;

        if(choice != 0)
        {
            choice--;
            if(choice != -1 && !validIndex(choice , hand.size()))
            throw runtime_error("Invaliv card");

            boost = hand[choice].getBoost();

            cout << "Using "<< hand[choice].getName()<< " (Boost = "<< boost<< ")\n";
        }
        else
        {
            cout << "No Boost selected.\n";
        }

        int movemax = hero->getmovement() + boost;

        cout << "Maximum movement = "<< movemax << endl;
    
         vector<Fighter*> movable;

        if(hero->getName()=="Sherlock")
        {
            movable.push_back(hero);
            movable.push_back(&battle.getWatson());
        }
        else
        {
            movable.push_back(hero);

            vector<Sisters>& sisters = battle.getsisters();

            for(auto& s : sisters)
            {
                if(s.isalive())
                movable.push_back(&s);
            }

        }

        vector<bool> moved(movable.size(),false);

        int movedCount=0;

        while(movedCount < movable.size())
        {
            for(int i=0;i<movable.size();i++)
            {
                if(moved[i])
                    continue;

                cout<<i+1<<") "<<movable[i]->getName();

                if(movable[i]->getName()=="Sister")
                    cout<<" "<<i;

                cout<<endl;
            }

            int choice;
            choice = readInt("\nChoose fighter to move ( 0 end):" , 0 , movable.size());

            if(choice==0)
                break;

            choice--;

            if(moved[choice])
            {
                cout<<"Already moved.\n";
                continue;
            }

            Fighter* selected = movable[choice];

            vector<Zone*> moves = battle.getReachableZone(*selected,movemax);

            cout<<"Possible destinations:\n";

            vector<int>validids;
            for(auto z:moves)
            {
              cout<<z->getId()<<" ";
              validids.push_back(z->getId());
            }

            cout<<endl;

            int dest;
            dest = readchoice ("Destination: " , validids);

            if(battle.movefighter(*selected,dest,movemax))
            {
                moved[choice]=true;
                movedCount++;
            }
            else
            {
                cout<<"Invalid move.\n";
            }
        }
}

void Player::attack(Player& enemy, Battle& battle,Fighter* attacker , int cardindex)
{
    if(attacker == nullptr || !attacker->isalive())
    {
        cout<<"This fighter can't attack\n";
        return;
    }
    if(hero == nullptr || enemy.getHero() == nullptr)
        throw logic_error("No fighter selected ..");

    cout << attacker->getName() << " ATTACKS... " 
         << enemy.getHero()->getName() << endl;

    battle.combat(attacker, enemy.getHero() , hero , cardindex);
}

   void Player::playScheme(Player& enemy, Battle& battle,Fighter* attacker , int cardindex)
    {
        Card schemecard = hero->playcard(cardindex);

        battle.applycardeffect(schemecard,attacker,enemy.getHero());

        cout << "Card execution Scheme\n";
    }
    void Player:: setHero(Fighter*  h )
    {
              hero = h ;
    }

    Fighter* Player:: chooseAttacker( Battle& battle ,  Card& attackCard , Fighter* opponent )
    {
        string owner  = attackCard.getowner();

         Fighter* hero = getHero();

         if(owner=="Dracula")
        {
            return &battle.getDracual();
        }

        
         if(owner=="Sherlock")
        {
            return &battle.getSherlock();
        }

         if(owner=="Watson")
        {
            return &battle.getWatson();
        }


        if(owner=="Sister")
        {
            vector<Fighter*> choices;

            for(auto& s : battle.getsisters())
            {
                if(s.isalive() && battle.areadjacent(s,*opponent))
                {
                    choices.push_back(&s);
                }
            }

            if(choices.empty())
                return nullptr;

           for(int i = 0; i < (int)choices.size(); i++)
            {
                cout << i + 1 << ") Sister ";
                    for(int j = 0; j < (int)battle.getsisters().size(); j++)
                    {
                        if(choices[i] == &battle.getsisters()[j])
                        {
                            cout << j + 1;
                            break;
                        }
                    }

                cout << endl;
            }
        }

        
            if(owner == "Any" && attackCard.getcardType() == SCHEME)
                {
                    cout << "Choose who will use this Scheme:\n";
                    cout << "1) Sherlock\n";
                    if(battle.getWatson().isalive())
                    cout << "2) Watson\n";

                    int max = battle.getWatson().isalive() ? 2 :1;
                    int ch = readInt("Choice: ",1,max);

                    if(ch == 1)
                        return &battle.getSherlock();

                    return &battle.getWatson();
                }

      if(owner == "Any")
        {
            vector<Fighter*> choices;

            if(hero->getName() == "Sherlock")
            {
                vector<Zone*> zones =battle.getMap().getplacementZone(hero->getPosition());

                for(auto z : zones)
                {
                    if(z == opponent->getPosition())
                    {
                        choices.push_back(hero);
                        break;
                    }
                }

                Fighter& watson = battle.getWatson();

                if(watson.isalive())
                {
                    vector<Zone*> wzones =  battle.getMap().getplacementZone(watson.getPosition());

                    for(auto z : wzones)
                    {
                        if(z == opponent->getPosition())
                        {
                            choices.push_back(&watson);
                            break;
                        }
                    }
                }
            }

            if(hero->getName() == "Dracula")
            {
                if(battle.areadjacent(battle.getDracual(), *opponent))
                    choices.push_back(&battle.getDracual());

                for(auto& s : battle.getsisters())
                {
                    if(s.isalive() &&
                    battle.areadjacent(s, *opponent))
                    {
                        choices.push_back(&s);
                    }
                }
            }

            for(int i = 0; i < (int)choices.size(); i++)
            {
                cout << i + 1 << ") "
                    << choices[i]->getName();

                if(choices[i]->getName() == "Sister")
                {
                    for(int j = 0; j < (int)battle.getsisters().size(); j++)
                    {
                        if(choices[i] == &battle.getsisters()[j])
                        {
                            cout << " " << j + 1;
                            break;
                        }
                    }
                }

                cout << endl;
            }

            int ch = readInt("Attack with: ", 1, choices.size());

            return choices[ch - 1];

        }
        return hero;
    }

    bool Player::chooseAttackerIfNeeded(Battle& battle,Card& card,Fighter*& attacker , Fighter* opponent)
{
    if(card.getcardType()==SCHEME)
    {
        if(card.getowner() == "Any")
        {
          attacker = chooseAttacker(battle , card ,opponent);
             
        }
        else
        {

            attacker = getHero();
        }
        return true;
    }

    attacker = chooseAttacker(battle , card ,opponent);

    return true;
}


