#include "Gamecontroller.h"
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
        if(current->getHero()->getName() == "InvisibleMan")
         {
                battle.checkInvisibleFogAtTurnStart();
         }
                
        cout << "\nTurn : " << current->getName() << endl;

        StatusPanel::show(battle);
        HandPanel :: show(*current->getHero());

        Player* enemy = (current == &player1 ? &player2 : &player1);

        int action = 0;

        if (current->getHero()->getName() == "InvisibleMan" && current->getHero()->getPosition() == nullptr)
        {
            cout << "\n--- Invisible Man must reappear on the board! ---\n";
            vector<int> validIds;

            for (int id = 1; id <= 32; id++)
            {
                Zone* z = battle.getMap().getZone(id);
                if (z == nullptr) continue;

                Fighter* occ = battle.getfighterat(z);
                if (occ != nullptr && occ->getteam() != current->getHero()->getteam())
                {
                    continue; 
                }

                validIds.push_back(id);
                cout << id << " ";
            }
            cout << "\n";

            if (validIds.empty())
            {
                cout << "No valid zones! (This shouldn't happen usually).\n";
            } 
            else
            {
                int destId = 0;
                bool isValid = false;
                while (!isValid)
                {
                    destId = readInt("Choose zone ID to reappear: ", 1, 32);
                    for (int id : validIds) {
                        if (id == destId) {
                            isValid = true;
                            break;
                        }
                    }
                    if (!isValid) cout << "Invalid zone. Try again.\n";
                }

                Zone* target = battle.getMap().getZone(destId);
                current->getHero()->setPosition(target);
                cout << "Invisible Man reappears at Zone " << target->getId() << "!\n";
            }
        }

        while (action < 2 && !battle.isgameover())
        {
            try
            {
                vector<AttackCardInfo> cards = current->getHero()->getPlayableCardIndexes(&battle, enemy->getHero(), current->getHero());
                
                for(int i = 0; i < cards.size(); i++)
                {
                    Card& c = current->getHero()->gethand()[cards[i].index];
                    cout << i+1 << ") " << c.getName();
                    if(!cards[i].usable)
                        cout << " [unusable]";
                    cout << "\n";
                }
                
                int input = readInt("Choose action:( or (0) for Maneuver)", 0, cards.size());
                
                if(input == 0)
                {
                    current->maneuver(battle);
                    StatusPanel::show(battle);
                    action++; 
                    continue;
                }

                input--;

                AttackCardInfo selected = cards[input];

                if(!selected.usable)
                {
                    cout << "This card is unusable\n";
                    continue;
                }

                Card& chosenCard = current->getHero()->gethand()[selected.index];

                Fighter* attacker = nullptr;

                if (!current->chooseAttackerIfNeeded(battle, chosenCard, attacker, enemy->getHero()) || attacker == nullptr) {
                    cout << "No valid fighter to play this card! Action cancelled.\n";
                    continue; 
                }

                if(chosenCard.getcardType() == SCHEME)
               {
                if (attacker == nullptr)
                {
                    attacker = current->getHero();
                }

                current->playScheme(*enemy, battle, attacker, selected.index);
                StatusPanel::show(battle);
                if (current->getHero()->getName() == "InvisibleMan" && current->getHero()->getPosition() == nullptr)
                {
                    cout << "Invisible Man has vanished! The turn ends immediately.\n";
                    action = 2; 
                    break;
                }
                action++; 
                continue; 
            }

             
                else if(chosenCard.getcardType() == ATTACK || chosenCard.getcardType() == VERSATILE)
                {
                    current->attack(*enemy, battle, attacker, selected.index);
                    StatusPanel::show(battle);
                }

                if (!player1.getHero()->isalive() || !player2.getHero()->isalive())
                {
                    battle.setGameOver(true);
                    cout << "\n*** GAME OVER! A main hero has been defeated! *\n";
                    break; 
                }
                
                if(battle.isgameover())
                    break;

                action++; 
                if(battle.hasExtraAction())
                {
                   action = 0; 
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