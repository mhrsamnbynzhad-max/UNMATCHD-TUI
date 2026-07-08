#include "GameController.h"
#include <iostream>
using namespace std;

GameController::GameController(Battle& b, Player& p1, Player& p2)
    : battle(b), player1(p1), player2(p2)
{
}

void GameController::run()
{
    std::queue<Player*> turnQueue;
    turnQueue.push(&player1);
    turnQueue.push(&player2);

    while (true)
    {
        Player* current = turnQueue.front();
        turnQueue.pop();
        
        cout << "\nTurn : " << current->getName() << endl;
        cout << "Choose action:\n";
        
        Player* enemy = (current == &player1 ? &player2 : &player1);

        int action = 0;
        while (action<2)
        {
           
        try
        {
          vector<AttackCardInfo> cards = current->getHero()->getAttackCardIndexes(&battle, enemy->getHero());

            for(int i = 0; i < cards.size(); i++)
            {
                Card& c = current->getHero()->gethand()[cards[i].index];

            cout << i+1 << ") "
                << c.getName();

            if(!cards[i].usable)
                cout<<" [unusable]";

                cout<<"\n";
            }
            
            
            int choice;
            cin >> choice;
            choice--;

            AttackCardInfo selected = cards[choice];

            if(!selected.usable)
            {
                cout<<"This card is unusable\n";
                continue;
            }


            Card& chosenCard = current->getHero()->gethand()[selected.index];


            if(chosenCard.getcardType() == SCHEME)
            {
                 current->playScheme(*enemy,battle, selected.index);
            }
            else if(chosenCard.getcardType() == ATTACK ||
                    chosenCard.getcardType() == VERSATILE)
            {
                current->attack(*enemy,battle, cards[choice].index);
            }

           action++;
        }
        catch(const exception& e)
        {
            cout << "Error: " << e.what() << endl;
        }
    }

    cout<<"Turn end? (1)\n";
    int end;
    cin>>end;
    while (end != 1)
    {
    cout<<"Confirm end turn :\n";  
    }

        turnQueue.push(current);
    }
}