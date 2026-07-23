#include "battle.h"
#include "cardfactory.h"
#include "cardeffect.h"
#include <iostream>
#include <algorithm>
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

        setuppositions();
        if(age1 < age2)
        {
            cout << player1.getName()
                << " is younger and chooses first.\n";
            player1.chooseHero(&sherlock, &dracula);
            chooseSidekickPosition(player1);

            if(player1.getHero() == &sherlock)
                player2.setHero(&dracula);
            else
                player2.setHero(&sherlock);

                chooseSidekickPosition(player2);
        }
        else if(age2 < age1)
        {
            cout << player2.getName()
                << " is younger and chooses first.\n";

            player2.chooseHero(&sherlock, &dracula);
                chooseSidekickPosition(player2);

            if(player2.getHero() == &sherlock)
                player1.setHero(&dracula);
            else
                player1.setHero(&sherlock);

            chooseSidekickPosition(player1);

        }
        else
        {
            cout << "Same age. Player 1 chooses first.\n";
            player1.chooseHero(&sherlock, &dracula);
            chooseSidekickPosition(player1);

            if(player1.getHero() == &sherlock)
                player2.setHero(&dracula);
            else
                player2.setHero(&sherlock);

                chooseSidekickPosition(player2);

        }

       cout<<"Player 2 atuomatically gets "<<player2.getHero()->getName()<<"!\n";

        dracula.setdeck(CardFactory::createDraculaDeck());
    
        sherlock.setdeck(CardFactory::createSherlockDeck());

        player1.drawCard();
        player2.drawCard();
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
    int choice;

    cout << "Use Sherlock Ability? ( 1 (Yes)/ 2 (No): ";
    cin >> choice;

    if(choice == 1)
    {
        sherlockAbilityActive = true;

        cout << "Sherlock ability activated.\n";
    }
}

void Battle::useDraculaAbility()
{
    int choice;

    cout << "Use Dracula Ability? ( 1(Yes) / 0 (No) ): ";
    cin >> choice;

    if(choice != 1 )
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

  ExecuteOrder Battle::getexecuteCardeffect(Card& attackCard,Card& defendCard,Fighter* attacker,Fighter* defender,bool defended)
{
    ExecuteOrder order;

    if(!defended)
    {
        order.acard = &attackCard;
        order.aowner = attacker;
        order.atarget = defender;

        order.bcard = nullptr;
        order.bowner = nullptr;
        order.btarget = nullptr;

        return order;
    }

    if(attackCard.getPriority() < defendCard.getPriority())
    {
        order.acard = &attackCard;
        order.aowner = attacker;
        order.atarget = defender;

        order.bcard = &defendCard;
        order.bowner = defender;
        order.btarget = attacker;
    }
    else
    {
        order.acard = &defendCard;
        order.aowner = defender;
        order.atarget = attacker;

        order.bcard = &attackCard;
        order.bowner = attacker;
        order.btarget = defender;
    }

    return order;
}