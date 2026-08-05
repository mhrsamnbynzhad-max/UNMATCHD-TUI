#include "battle.h"
#include "cardfactory.h"
#include "cardeffect.h"
#include <iostream>
#include <algorithm>
#include <queue>
#include <set>
#include "handling.h"
#include "fighter.h"


using namespace std;
    Battle::Battle():  sherlock(),  watson(),  dracula() ,sisters(), player1("Player 1") , player2("Player 2")
    {
        for(int i = 0 ; i <3 ; i ++)
        {
          sisters.emplace_back();
        }
        int age1, age2;
        age1 = readInt(player1.getName()+" age: " , 1 , 100);
        age2 = readInt(player2.getName()+" age: ", 1 , 100);
        

        setuppositions();

        if(age1 < age2)
        {
            cout << player1.getName()
                << " is younger and chooses first.\n\n";
                playerfirst = true;
            player1.chooseHero(&sherlock, &dracula);
            chooseSidekickPosition(player1);

            if(player1.getHero() == &sherlock)
                player2.setHero(&dracula);
            else
                player2.setHero(&sherlock);


            cout<<"\n"<<player2.getName()<<" automatically gets " <<player2.getHero()->getName()<<"\n";

                chooseSidekickPosition(player2);
        }
        else if(age2 < age1)
        {
            cout << player2.getName()
                << " is younger and chooses first.\n";
                playerfirst = false;

            player2.chooseHero(&sherlock, &dracula);
                chooseSidekickPosition(player2);

            if(player2.getHero() == &sherlock)
                player1.setHero(&dracula);
            else
                player1.setHero(&sherlock);


               cout<<"\n"<<player1.getName()<<" automatically gets " <<player1.getHero()->getName()<<"\n";


               chooseSidekickPosition(player1);

        }
        else
        {
            cout << "Same age. Player 1 chooses first.\n";
            playerfirst = true;
            player1.chooseHero(&sherlock, &dracula);
            chooseSidekickPosition(player1);

            if(player1.getHero() == &sherlock)
                player2.setHero(&dracula);
            else
                player2.setHero(&sherlock);


            cout<<"\n"<<player2.getName()<<" automatically gets " <<player2.getHero()->getName()<<"\n";


                chooseSidekickPosition(player2);

        }

        dracula.setdeck(CardFactory::createDraculaDeck());
    
        sherlock.setdeck(CardFactory::createSherlockDeck());

        player1.drawCard();
        player2.drawCard();
    }

      void Battle::startTurn(Player& player)
        {
           sherlockAbilityActive = false;
 
        Fighter* hero = player.getHero();
        
        hero->specialAbillity(this);
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