#include "GameController.h"
#include <iostream>
#include <string>
#include "handpanel.h"
#include "handling.h"
#include "StatusPanel.h"
using namespace std;

GameController::GameController(Battle& b, Player& p1, Player& p2) : battle(b), player1(p1), player2(p2)
{
}

void GameController::run()
{
    std::queue<Player*> turnQueue;
    if(battle.getplayerfirst())
    {
        turnQueue.push(&player1);
        turnQueue.push(&player2);
    }
    else
    {
        turnQueue.push(&player2);  
        turnQueue.push(&player1);
    }

    while (true)
    {

        if(battle.isgameover())
        break;


        Player* current = turnQueue.front();
        turnQueue.pop();
        battle.startTurn(*current);
                
        cout << "\nTurn : " << current->getName() << endl;

        StatusPanel::show(battle);
        HandPanel :: show(*current->getHero());

        Player* enemy = (current == &player1 ? &player2 : &player1);
        
        int action = 0;

        while (action<2)
        {
            
            try
            {
              
             vector<AttackCardInfo> cards = current->getHero()->getPlayableCardIndexes(&battle, enemy->getHero(), current->getHero());
                
            for(int i = 0; i < cards.size(); i++)
            {
                Card& c = current->getHero()->gethand()[cards[i].index];

            cout << i+1 << ") "
                << c.getName();

            if(!cards[i].usable)
                cout<<" [unusable]";

                cout<<"\n";
            }
            int input;
            input = readInt( "Choose action:( or (0) for Maneuver)" , 0 , cards.size());
            
            if( input == 0 )
            {
                current->maneuver(battle);

                StatusPanel::show(battle);

                action++;
                continue;
            }

            input -- ;

            AttackCardInfo selected = cards[input];

            if(!selected.usable)
            {
                cout<<"This card is unusable\n";
                continue;
            }


            Card& chosenCard = current->getHero()->gethand()[selected.index];

            Fighter* attacker = nullptr;

            current->chooseAttackerIfNeeded(battle,chosenCard, attacker, enemy->getHero());


            if(chosenCard.getcardType() == SCHEME)
            {
                current->playScheme(*enemy,battle,attacker ,selected.index);

                StatusPanel::show(battle);
            }
            else if(chosenCard.getcardType() == ATTACK ||chosenCard.getcardType() == VERSATILE)
            {
                current->attack(*enemy,battle, attacker, cards[input].index);

                StatusPanel::show(battle);
            }
             if(battle.isgameover())
             break;


           action++;
           if(battle.hasExtraAction())
           {
              action = 1;
              battle.resetExtraAction();
           }
        }
        catch(const exception& e)
        {
            cout << "Error: " << e.what() << endl;
        }
    }

        if(battle.isgameover())
        return ;


        int end;
        end = readInt("Confirm end turn (1): " , 1 ,1);

        vector<Card>& hand = current->getHero()->gethand();

        while(hand.size()>7)
        {
            hand.pop_back();
        }

        turnQueue.push(current);
    }
}