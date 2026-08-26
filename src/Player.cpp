#include "Player.h"
#include "battle.h"
#include "handling.h"
#include "boardmanager.h"
#include "combatmanager.h"
#include <iostream>
#include <stdexcept>


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

void Player::chooseHero(const vector<Fighter*>& heroes, int choice)
{
    if (choice < 1 || choice > static_cast<int>(heroes.size()))
        throw std::runtime_error("Invalid hero choice");

    hero = heroes[choice - 1];
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

    for(int i = 0; i < hand.size(); i++)
    {
        cout << i + 1 << ") " << hand[i].getName() << " (Boost = " << hand[i].getBoost() << ")\n";
    }

    int boost = 0;
    int choice;
    choice = readInt("Choose a card to use its Boost (0 = No Boost): ", 0, hand.size());

    if(choice != 0)
    {
        choice--;
        if(!validIndex(choice, hand.size()))
            throw runtime_error("Invalid card");

        boost = hand[choice].getBoost();

        cout << "Using " << hand[choice].getName() << " (Boost = " << boost << ")\n";
        

        hand.erase(hand.begin() + choice);
    }
    else
    {
        cout << "No Boost selected.\n";
    }

    int movemax = hero->getmovement() + boost;

    cout << "Maximum movement = " << movemax << endl;

    vector<Fighter*> movable;

    for (Fighter* f : battle.getFighters())
    {
        if (f != nullptr && f->isalive() && f->getteam() == hero->getteam())
        {
            movable.push_back(f);
        }
    }

    vector<bool> moved(movable.size(), false);
    int movedCount = 0;

    while(movedCount < movable.size())
    {
        for(int i = 0; i < movable.size(); i++)
        {
            if(moved[i])
                continue;

            cout << i + 1 << ") " << movable[i]->getName();

            if(movable[i]->getName() == "Sister")
                cout << " " << i;

            cout << endl;
        }

        int choice;
        choice = readInt("\nChoose fighter to move (0 end): ", 0, movable.size());

        if(choice == 0)
            break;

        choice--;

        if(moved[choice])
        {
            cout << "Already moved.\n";
            continue;
        }

        Fighter* selected = movable[choice];

        vector<Zone*> moves = battle.getBoard()->getReachableZone(*selected, movemax);

        cout << "Possible destinations:\n";

        vector<int> validids;
        for(auto z : moves)
        {
            cout << z->getId() << " ";
            validids.push_back(z->getId());
        }

        cout << endl;

        int dest;
        dest = readchoice("Destination: ", validids);

        if(battle.getBoard()->movefighter(*selected, dest, movemax))
        {
            moved[choice] = true;
            movedCount++;
        }
        else
        {
            cout << "Invalid move.\n";
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

    battle.getCombat()->combat(attacker, enemy.getHero() , hero , cardindex);
}
void Player::playScheme(Player& enemy, Battle& battle, Fighter* attacker, int cardindex)
{
    if (cardindex < 0 || cardindex >= hero->gethand().size()) {
        cout << "Error: Invalid scheme card index!\n";
        return;
    }

    Card schemecard = hero->gethand()[cardindex];

    cout << "Card execution Scheme\n";

    battle.getCombat()->applycardeffect(schemecard, attacker, enemy.getHero());

    hero->playcard(cardindex);
}
    void Player:: setHero(Fighter*  h )
    {
              hero = h ;
    }

 Fighter* Player::chooseAttacker(Battle& battle, Card& attackCard, Fighter* opponent)
{
    string owner = attackCard.getowner();
    Fighter* hero = getHero();

    if (owner == "Dracula")
    {
        if (hero->getName() == "Dracula" && hero->isalive())
            return hero;
        return nullptr;
    }

    if (owner == "Sherlock")
    {
        if (hero->getName() == "Sherlock" && hero->isalive())
            return hero;
        return nullptr;
    }

    if (owner == "Watson")
    {
        Fighter& watson = battle.getWatson();
        if (watson.isalive() && battle.getPlayerOfFighter(&watson) != nullptr)
            return &watson;
        return nullptr;
    }

    if (owner == "Sister")
    {
        vector<Fighter*> choices;

        for (auto& s : battle.getsisters())
        {
            if (s.isalive() && battle.getBoard()->areadjacent(s, *opponent))
            {
                choices.push_back(&s);
            }
        }

        if (choices.empty())
            return nullptr;

        for (int i = 0; i < (int)choices.size(); i++)
        {
            cout << i + 1 << ") Sister ";
            for (int j = 0; j < (int)battle.getsisters().size(); j++)
            {
                if (choices[i] == &battle.getsisters()[j])
                {
                    cout << j + 1;
                    break;
                }
            }
            cout << endl;
        }

        int ch = readInt("Attack with Sister: ", 1, choices.size());
        return choices[ch - 1];
    }

    if (owner == "Any" && attackCard.getcardType() == SCHEME)
    {
        if (hero->getName() == "Sherlock")
        {
            cout << "Choose who will use this Scheme:\n";
            cout << "1) Sherlock\n";
            
            bool watsonAlive = battle.getWatson().isalive() && (battle.getPlayerOfFighter(&battle.getWatson()) != nullptr);
            if (watsonAlive)
                cout << "2) Watson\n";

            int maxChoice = watsonAlive ? 2 : 1;
            int ch = readInt("Choice: ", 1, maxChoice);

            if (ch == 1)
                return hero;
            return &battle.getWatson();
        }

        return hero;
    }

    if (owner == "Any")
    {
        vector<Fighter*> choices;

        if (hero->getName() == "Sherlock")
        {
            if (hero->isalive())
            {
                vector<Zone*> zones = battle.getMap().getplacementZone(hero->getPosition());
                for (auto z : zones)
                {
                    if (z == opponent->getPosition())
                    {
                        choices.push_back(hero);
                        break;
                    }
                }
            }

            Fighter& watson = battle.getWatson();
            if (watson.isalive() && battle.getPlayerOfFighter(&watson) != nullptr)
            {
                vector<Zone*> wzones = battle.getMap().getplacementZone(watson.getPosition());
                for (auto z : wzones)
                {
                    if (z == opponent->getPosition())
                    {
                        choices.push_back(&watson);
                        break;
                    }
                }
            }
        }

        if (hero->getName() == "Dracula")
        {
            if (hero->isalive() && battle.getBoard()->areadjacent(*hero, *opponent))
                choices.push_back(hero);

            for (auto& s : battle.getsisters())
            {
                if (s.isalive() && battle.getBoard()->areadjacent(s, *opponent))
                {
                    choices.push_back(&s);
                }
            }
        }

        if (hero->getName() == "InvisibleMan")
        {
            if (hero->isalive())
                choices.push_back(hero);
        }

        if (choices.empty())
            return nullptr;

        if (choices.size() == 1)
            return choices[0];

        for (int i = 0; i < (int)choices.size(); i++)
        {
            cout << i + 1 << ") " << choices[i]->getName();

            if (choices[i]->getName() == "Sister")
            {
                for (int j = 0; j < (int)battle.getsisters().size(); j++)
                {
                    if (choices[i] == &battle.getsisters()[j])
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
      if (card.getcardType() == SCHEME)
    {
        attacker = hero;  
        return true;
    }
    attacker = chooseAttacker(battle , card ,opponent);

    return attacker != nullptr;
}


