#include "battle.h"
#include "cardfactory.h"
#include "cardeffect.h"
#include <iostream>
#include <queue>
#include <queue>
#include <set>

using namespace std;

    Battle::Battle():  sherlock(),  watson(),  dracula() , player1("Player 1") , player2("Player 2")
    {
        for(int i=0 ; i<3 ; i++)
        {
            sisters.push_back(
                Fighter("Sister", 1 , 1, false , 2 , false)
            );
        }
        int age1, age2;

        cout << player1.getName() << " age: ";
        cin >> age1;

        cout << player2.getName() << " age: ";
        cin >> age2;

        if(age1 < age2)
        {
            cout << player1.getName()
                << " is younger and chooses first.\n";

            player1.chooseHero(&sherlock, &dracula);

            if(player1.getHero() == &sherlock)
                player2.setHero(&dracula);
            else
                player2.setHero(&sherlock);
        }
        else if(age2 < age1)
        {
            cout << player2.getName()
                << " is younger and chooses first.\n";

            player2.chooseHero(&sherlock, &dracula);

            if(player2.getHero() == &sherlock)
                player1.setHero(&dracula);
            else
                player1.setHero(&sherlock);
        }
        else
        {
            cout << "Same age. Player 1 chooses first.\n";

            player1.chooseHero(&sherlock, &dracula);

            if(player1.getHero() == &sherlock)
                player2.setHero(&dracula);
            else
                player2.setHero(&sherlock);
        }
       cout<<"Player 2 atuomatically gets "<<player2.getHero()->getName()<<"!\n";

        setuppositions();
        dracula.setdeck(CardFactory::createDraculaDeck());
    
        sherlock.setdeck(CardFactory::createSherlockDeck());

            player1.drawCard();
            player2.drawCard();
            cout << player1.getName()
            << " -> "
            << player1.getHero()->getName()
            << endl;

            cout << player2.getName()
                << " -> "
                << player2.getHero()->getName()
                << endl;
    }

   void Battle::startTurn(Player& player)
{
    sherlockAbilityActive = false;

    Fighter* hero = player.getHero();

    if(hero == &dracula)
    {
        useDraculaAbility();
    }
    else if(hero == &sherlock)
    {
        useSherlockAbility();
    }
}

void Battle::useSherlockAbility()
{
    char choice;

    cout << "Use Sherlock Ability? (y/n): ";
    cin >> choice;

    if(choice == 'y' || choice == 'Y')
    {
        sherlockAbilityActive = true;

        cout << "Sherlock ability activated.\n";
    }
}

void Battle::useDraculaAbility()
{
    char choice;

    cout << "Use Dracula Ability? (y/n): ";
    cin >> choice;

    if(choice != 'y' && choice != 'Y')
        return;

    Zone* currentZone = dracula.getPosition();

    vector<Fighter*> targets;

    for(Zone* neighbor : currentZone->getNei())
    {
        if(sherlock.isalive() &&
           sherlock.getPosition() == neighbor)
        {
            targets.push_back(&sherlock);
        }

        if(watson.isalive() &&
           watson.getPosition() == neighbor)
        {
            targets.push_back(&watson);
        }

        for(Fighter& sister : sisters)
        {
            if(sister.isalive() &&
            sister.getPosition() == neighbor)
            {
                targets.push_back(&sister);
            }
        }
    }

    if(targets.empty())
    {
        cout << "No valid targets.\n";
        return;
    }

    cout << "Choose target:\n";

    for(size_t i = 0; i < targets.size(); i++)
    {
        cout << i + 1
             << ". "
             << targets[i]->getName()
             << endl;
    }

    int target;
    cin >> target;

    if(target >= 1 &&
       target <= targets.size())
    {
        targets[target - 1]->takeDamage(1);
    }
}