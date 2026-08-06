#include "battle.h"
#include "cardfactory.h"
#include "cardeffect.h"
#include <iostream>
#include <algorithm>
#include <queue>
#include <set>
#include <cstdlib>
#include <ctime>
#include "handling.h"
#include "fighter.h"


using namespace std;
    Battle::Battle():  sherlock(),  watson(),  dracula() ,sisters(),invisibleman(),fogtoken() ,player1("Player 1") , player2("Player 2")
    {
        srand(time(0));
        for(int i = 0 ; i <3 ; i ++)
        {
          sisters.emplace_back();
        }
        for(int i = 0 ; i <3 ; i ++)
        {
          fogtoken.push_back(FogToken());
        }

        int age1, age2;
        age1 = readInt(player1.getName()+" age: " , 1 , 100);
        age2 = readInt(player2.getName()+" age: ", 1 , 100);
        

        setuppositions();

        vector<Fighter*> heroes ={ &sherlock, &dracula, &invisibleman};
        if(age1 < age2)
        {
            cout << player1.getName()<< " is younger and chooses first.\n\n";
                playerfirst = true;
                chooseHeroes(player1, player2); 
        }
        else if(age2 < age1)
        {
            cout << player2.getName() << " is younger and chooses first.\n";
            playerfirst = false;
            chooseHeroes(player2, player1);

        }
        else
        {
            playerfirst = rand()%2;
            if(playerfirst)
            {
            cout << "Same age. Player 1 chooses first.\n";

                chooseHeroes(player1, player2);
            }
            else
            {
            cout << "Same age. Player 2 chooses first.\n";

                chooseHeroes(player2, player1);
                
            }


    
        }

        dracula.setdeck(CardFactory::createDraculaDeck());
    
        sherlock.setdeck(CardFactory::createSherlockDeck());

        player1.drawCard();
        player2.drawCard();
    }

    void Battle::chooseHeroes(Player& first, Player& second)
    {
         vector<Fighter*> heroes ={  &sherlock, &dracula, &invisibleman};

    first.chooseHero(heroes);

    heroes.erase(remove(heroes.begin(), heroes.end(), first.getHero()),  heroes.end());

    first.getHero()->setupUnits(this, first);

    second.chooseHero(heroes);

    second.getHero()->setupUnits(this, second);
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