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
        player1.chooseHero(&sherlock , &dracula);
       if(player1.getHero() == &sherlock)
       {
         player2.setHero(&dracula);
       }
       else
       {
         player2.setHero(&sherlock);
          
       }
       cout<<"Player 2 atuomatically gets "<<player2.getHero()->getName()<<"!\n";

        setuppositions();
        dracula.setdeck(CardFactory::createDraculaDeck());
    
        sherlock.setdeck(CardFactory::createSherlockDeck());

        player1.drawCard();
        player2.drawCard();
    }

   