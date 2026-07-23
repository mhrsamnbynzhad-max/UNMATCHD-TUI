#include "GameController.h"
#include <iostream>
#include <string>
#include "handpanel.h"
#include "StatusPanel.h"
using namespace std;

GameController::GameController(Battle& b, Player& p1, Player& p2) : battle(b), player1(p1), player2(p2)
{
}

void GameController::run()
{
    std::queue<Player*> turnQueue;
    turnQueue.push(&player1);
    turnQueue.push(&player2);

    while (true)
    {
        if(!battle.getSherlock().isalive())
        {
                cout << "\nGame Over!\n";
                cout << battle.getDracual().getName() << " wins!\n";
                break;
        }

        if(!battle.getDracual().isalive())
        {
                cout << "\nGame Over!\n";
                cout << battle.getSherlock().getName() << " wins!\n";
                break;
        }
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
                
            cout << "Choose action:( or M for Maneuver)\n";
            for(int i = 0; i < cards.size(); i++)
            {
                Card& c = current->getHero()->gethand()[cards[i].index];

            cout << i+1 << ") "
                << c.getName();

            if(!cards[i].usable)
                cout<<" [unusable]";

                cout<<"\n";
            }
            string input;
            cin>>input;
            if( input == "M" || input == "m")
            {
                current->maneuver(battle);

                StatusPanel::show(battle);

                action++;
                continue;
            }
            int choice = stoi(input);
            choice--;

            AttackCardInfo selected = cards[choice];

            if(!selected.usable)
            {
                cout<<"This card is unusable\n";
                continue;
            }


            Card& chosenCard = current->getHero()->gethand()[selected.index];

            Fighter* attacker = nullptr;

            current->chooseAttackerIfNeeded(battle,chosenCard,attacker);


            if(chosenCard.getcardType() == SCHEME)
            {
                current->playScheme(*enemy,battle, selected.index);

                StatusPanel::show(battle);
            }
            else if(chosenCard.getcardType() == ATTACK ||
                    chosenCard.getcardType() == VERSATILE)
            {
                current->attack(*enemy,battle, attacker, cards[choice].index);

                StatusPanel::show(battle);
            }

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

        cout<<"Turn end? (1)\n";

        int end;
        cin>>end;

        while(end != 1)
        {
            cout<<"Confirm end turn (1): ";
            cin>>end;
        }

        turnQueue.push(current);
    }
}