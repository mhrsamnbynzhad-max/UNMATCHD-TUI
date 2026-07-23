#include  <iostream>
#include "card.h"
#include "fighter.h"
#include "battle.h"
#include "zone.h"
#include "cardeffect.h"

using namespace std;

void BloodThirstEffect :: apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card)
  {
                Zone* enemyZone = defender->getPosition();          // Place of defender
                char color = enemyZone->getColor();                 // Color of defender place

                vector<Zone*> sameColorZones = battle->getMap().getZonebycolor(color);   // All places that is the same color 

                int counter = 0;

                for (const Fighter& sis : battle->getsisters())                 
                {
                    Zone* sisPos = sis.getPosition();               // Sister's place

                    for (Zone* zone : sameColorZones)               // Check the places that share the same color as the defender's place 
                    {
                        if (sisPos->getId() == zone->getId())       // If a sister is found on any of the same-colored places, increase the counter
                        {
                            counter++;
                            if (counter == 3) break;                // The maximum allowed value for the counter is 3 .
                        }
                    }
                    if (counter == 3) break;
                }

               card.setValue(card.getValue()+ counter);                       // Increases attack 
 }
       
void AmbushEffect :: apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card)
{
        if(!defender || defender->handsize() == 0)
        {
            return;
        }

        int  index = rand() % defender->handsize();  
        Card remove = defender->remove_ranodmcard();

        int opponentBoost = remove.getBoost();
        card.setValue(card.getValue() +  opponentBoost);

        cout<<"Ambush : opponent discarded "<<remove.getName()<<" and + " <<opponentBoost <<" attack added . \n";

}

 void FeastEffect :: apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card)
{
        attacker->heal(2);
        cout<<"Feast: Dracula healed +2 HP. \n";

        vector<Fighter>& sisters = battle->getsisters();
        sisters[1].sethealth(0);
        cout<<"test sister 1 dead\n";
        vector<int> deadindexes;

        for(int i = 0 ; i< (int)sisters.size(); i++)
        {
            if(!sisters[i].isalive())
            {
                deadindexes.push_back(i);
            }
        }

        if(deadindexes.empty())
        {
            cout<<"Feast : No ddead sister to revive.\n";
            return;
        }
        int idx = deadindexes[0];
        Fighter& revived = sisters[idx];

        revived.heal(1);

       Zone* draculaZone = attacker->getPosition();
       revived.setPosition(draculaZone);

       cout<<"Feast : "<<revived.getName()<<"revived with 1 hp at dracula's zone.\n ";

}

void MistFormEffect::apply(Fighter* attacker,
                           Fighter* defender,
                           Battle* battle,
                           Card& card)
{
    cout << "\nMist Form activated!\n";

    Fighter& dracula = battle->getDracual();

    cout << "Choose a zone for Dracula:\n";

    vector<Zone*> zones = battle->getMap().getplacementZone(dracula.getPosition());

    for(int i = 0; i < zones.size(); i++)
    {
        if(battle->getfighterat(zones[i]) == nullptr)
        {
            cout << zones[i]->getId() << " ";
        }
    }

    cout << endl;

    int choice;
    cin >> choice;


    Zone* newZone = battle->getMap().getZone(choice);


    if(battle->getfighterat(newZone) == nullptr)
    {
        dracula.setPosition(newZone);

        cout << "Dracula moved to zone "
             << newZone->getId()
             << endl;
             battle->giveExtraAction();
    }
    else
    {
        cout << "Invalid zone!\n";
        return;
    }


    cout << "\nDracula gains an extra action!\n";

    cout << "Choose action:\n";
    cout << "1) Attack\n";
    cout << "2) Maneuver\n";
    cout << "3) Scheme\n";

    int action;
    cin >> action;


    if(action == 1)
    {
        cout << "Extra Attack action selected\n";
    }
    else if(action == 2)
    {
        cout << "Extra Maneuver action selected\n";
    }
    else if(action == 3)
    {
        cout << "Extra Scheme action selected\n";
    }
}
 void MonesterFormEffect :: apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card) 
 {
    cout<<"hand size beformonesform "<<attacker->gethand().size()<<endl;
    int basattack = card.getValue();
    int bouns = 0;
    vector<Card>& hand = attacker->gethand();
    cout<<" you may discard  cards too increase attack\n";

    while (true)
    {
        if(hand.empty())
        {
            cout<<"your hand is  empty no mmore discard possible\n";
            break;
        }
        cout<<"\nyour hand\n";
        for(int  i = 0 ; i <hand.size(); i ++)
        {
            cout<< i+1 <<"]"<<hand[i].getName()<<"\n";
        }
            cout<<"enter your card index to discard(or 0 to stop)\n";
            int choose;
            cin>>choose;
            choose --;
            if(choose == -1)
            {
                cout<<"you stopped dicarding \n";
                break;
            }
            if(choose<-1 || choose >= hand.size())
            {
                cout<<"invalid...\n";
                continue;
            }
            cout<<"discard : "<<hand[choose].getName()<<"\n";
            hand.erase(hand.begin()+choose);
            bouns++;
            cout<<"attack increases by +1 (bonus = )"<<bouns<<"\n";
        }
            int lastattack = basattack + bouns;
              cout<<"final attack : " <<lastattack<<"\n";
              card.setValue(lastattack);  
 }

  void  ManeuverEffect :: apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card) 
  {
    if(attacker->getName() != "Dracula")
    {
        cout<<"Only Dracula can move 3 palces. \n";
        return;
    }

    Zone* current  = attacker->getPosition();

    int moves = 0;

    while (moves <3)
    {
       cout<<"\n-----Dracula movment------\n";
       cout<<"Current Zone : "<<current->getId()<<"\n";

       vector<Zone*> connectedzone = current->getNei();

       cout<<"Available connected zones : \n";
       vector<Zone*> validmoves;
      
       for(Zone* z : connectedzone)
       {
            Fighter* Novalid = battle->getfighterat(z);
            int moveleftafter = 3 - (moves + 1);
            if(Novalid != nullptr)
            {
                auto check = battle->canEnterzone(attacker , Novalid , moveleftafter);
                if(!check.allow)
                {
                    cout<<"Zone "<<z->getId() <<" Blocked";
                    if(!check.blocker.empty())
                     cout<<" ( blocked by  "<<check.blocker<<" )";
                     cout<<"\n";
                    continue;
                }

                if(check.blocker == "Sister")
                {
                  cout<<"Zone "<<z->getId() <<" Passable (Sister)\n";
                  validmoves.push_back(z);
                  continue;
                    
                } 
            }
                validmoves.push_back(z);
                cout<<"Zone "<<z->getId()<<"(OK)\n";
        }
         if(validmoves.empty())
         {
            cout<<"No valid moves ..\n";
            break;
         }
         cout<<"Enter zone ID to move Dracual :";
         int choice;
         cin>>choice;
         Zone* selected  = nullptr;
         for (Zone* z : validmoves)
         {
            if(z->getId() == choice)
            {
                selected = z ;
                break;
            }
         }
         if(selected == nullptr)
         {
              cout<<"Invalid zone .try again \n";
              continue;
         }

         attacker->setPosition(selected);
         current = selected;
         cout<<"Dracula moved to zone "<<current->getId()<<"\n";

         moves++;
         Fighter* no = battle->getfighterat(current);

         if( no && no->getName() == "Sister" && moves < 3)
         {
            cout<<" You are On a sister . you must keep moving\n";
            continue;
         }

         if(moves>= 3)
         {
            cout<<"Maximum movement reched (3).\n";
            break;
         }
          cout<<"Do you want to  continue moving ? ( yees (1) or NO (0). :\n";
          int ok;
          cin>> ok;
          if( ok == 0)
          {
             cout<<"Movement  sttoped  by player .\n";
             break;
          }
    }
  }

  void  ExploitEffect :: apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card) 
  {
    auto cards = attacker->getrandomcard(1);
    if(!cards.empty())
    {
        attacker->addtohand(cards);
        cout<<"Draw a card . added to your hand .\n";
    }
  }

  void LookIntoMyEyesEffect :: apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card)
  {
    int oppBoost = battle->gelastattackcard().getBoost();

    card.setValue(card.getValue() + oppBoost);

    cout<<" opponent  boost ("<<oppBoost<<" 0 added to this defense.\n";
  }

void HuntEyesEffect :: apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card) 
{
  Zone* dracula = attacker->getPosition();
  vector<Zone*> connected = dracula->getNei();

  int  healcount = 0;

  for (Zone* place : connected)
  {
    Fighter* novalid = battle->getfighterat(place);

    if(novalid != nullptr)
    {
        if(novalid->isheroteam() != attacker->isheroteam())
        {
            cout<<"Hunt : opponent found in zone "<< place->getId()<<"\n";

            novalid->takeDamage(1);
            cout<<novalid->getName()<<" take 1 damage .\n";

            healcount++;
        }
    }
  }
  
 for (int i = 0; i < healcount; i++)
 {
    cout<<"dracula hp "<<attacker->getHealth()<<"/"<<attacker->getMaxealth()<<endl;
    if(attacker->getHealth() < attacker->getMaxealth())
    {
        attacker->heal(1);
        cout<<"Dracula haled 1 HP (current HP : "<<attacker->getHealth()<<")\n";
    }

 }
 
}

void SeductivecallEffect :: apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card)
{
    vector<Fighter*> fighters;
    fighters.push_back(&battle->getDracual());
    fighters.push_back(&battle->getSherlock());
    fighters.push_back(&battle->getWatson());

    for(auto& sisi : battle->getsisters())
    {
        fighters.push_back(&sisi);
    }

    cout<<" which one fighter do you want to move?\n";
    for (int i = 0; i < fighters.size(); i++)
    {
        cout<< i+1 <<" )"<<fighters[i]->getName()<<"  (Zone "<<fighters[i]->getPosition()->getId()<<" )\n";
    }

    int choose;
    cin>>choose;
    choose--;
    if(choose < 0 || choose>= fighters.size())
    {
        cout<<"Invalid choose\n";
        return ;
    }
   
    Fighter* selected = fighters[choose];

    int moves = 0;

    while ( moves <2)
    {
       cout<<"Move ? ( yes (1) , no (0))";
       int ok;
       cin>> ok;
       if(ok == 0 )
       {
         cout<<" stoped move\n";
         break;
       }
       Zone* current = selected->getPosition();
       vector<Zone*>connected = current->getNei();
       cout<<" Connected zones : ";
       for(Zone* z : connected)
       {
        cout<<z->getId()<<" ";
       }
       int dest;
       cin>>dest;
       bool valid = battle->movefighter(*selected,dest ,1);

       if(!valid)
       {
          cout<<"Invalid move..\n";
          continue;
       }

       moves++;
       
       if(moves == 2)
       {
           cout<<"Maximum movement reched (2)\n";
           break;
        }
    }

       if(selected->getName() == "Sister")
       {
          cout<<" no damage , fighter selected for moove is sister .\n";
          return;
       }
       Zone* now = selected->getPosition();
       vector<Zone*>connected = now->getNei();

       bool sisternearby = false;
       for(Zone* z : connected)
       {
         Fighter* occ = battle->getfighterat(z);
         if(occ && occ->getName() == "Sister")
         {
            sisternearby = true;
         }

       }

       if(sisternearby)
       {
          cout<<" found a sister "<<selected->getName()<<" take 1 damage .\n";
          selected->takeDamage(1);

       }
       else{
        cout<<" no found sister.\n";
       }
}

 void  SurvivalInstinctEffect :: apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card)
 {
     // 1) Calculate combat result
    int finalAttack = card.getValue();
    int finalDefense = battle->getlastdefend();

    cout << "Final Attack = " << finalAttack << " | Final Defense = " << finalDefense << endl;

    if(finalAttack <= finalDefense)
    {
        cout << "The Sister did not win the combat. Nothing happens.\n";
        return;
    }

    cout << "The Sister won the combat! Dracula may be placed next to the enemy fighter.\n";

    // 2) Choose opponent target (Sherlock or Watson)
    vector<Fighter*> opponent;
    opponent.push_back(&battle->getSherlock());
    opponent.push_back(&battle->getWatson());

    cout << "Dracula wants to move next to which opponent?\n";
    for(int i = 0; i < opponent.size(); i++)
    {
        cout << i+1 << ") " << opponent[i]->getName()
             << " (Zone " << opponent[i]->getPosition()->getId() << ")\n";
    }

    int choice;
    cin >> choice;
    choice--;

    if(choice < 0 || choice >= opponent.size())
    {
        cout << "Invalid choice.\n";
        return;
    }

    Fighter* targetopponent= opponent[choice];
    Zone* opponentZone = targetopponent->getPosition();

    // 3) Find adjacent zones to the opponent
    vector<Zone*> neighbors = opponentZone->getNei();
    vector<Zone*> emptyZones;

    cout << "Adjacent zones to the enemy:\n";
    for(Zone* z : neighbors)
    {
        Fighter* occ = battle->getfighterat(z);
        cout << "Zone " << z->getId();

        if(occ == nullptr)
        {
            cout << " (Empty)\n";
            emptyZones.push_back(z);
        }
        else
        {
            cout << " (Occupied by " << occ->getName() << ")\n";
        }
    }


    if(emptyZones.empty())
    {
        cout << "There are no empty adjacent zones. Dracula cannot be moved.\n";
        return;
    }

    // 4) Choose empty zone for Dracula
    cout << "Dracula can move to these empty zones:\n";
    for(int i = 0; i < emptyZones.size(); i++)
    {
        cout << i+1 << ") Zone " << emptyZones[i]->getId() << endl;
    }

    int destChoice;
    cin >> destChoice;
    destChoice--;

    if(destChoice < 0 || destChoice >= emptyZones.size())
    {
        cout << "Invalid choice.\n";
        return;
    }

    Zone* finalZone = emptyZones[destChoice];

    // 5) Move Dracula
    Fighter& dracula = battle->getDracual();
    dracula.setPosition(finalZone);

    cout << "Dracula moved to Zone " << finalZone->getId() << ".\n";
 }

 void FeintEffect :: apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card)
 {
    cout<<"Opponent card's effect is cancelled\n";
    battle->setCancel(true);
 }



// ---------------- Counter Attack ----------------

void CounterAttackEffect::apply(Fighter* attacker,
                                Fighter* defender,
                                Battle* battle,
                                Card& card)
{
    if(!attacker || !defender)
        return;


    if(battle->areadjacent(*attacker,*defender))
    {
        defender->takeDamage(2);

        cout<<"Counter Attack: "
            <<defender->getName()
            <<" takes 2 damage.\n";
    }
    else
    {
        cout<<"Counter Attack failed: fighters are not adjacent.\n";
    }
}



// ---------------- Fixed Point ----------------
void FixedPointEffect::apply(Fighter* attacker,  Fighter* defender,  Battle* battle,  Card& card)
{
    Fighter& sherlock = battle->getSherlock();
    Fighter& watson = battle->getWatson();

    if(battle->areadjacent(sherlock, watson))
    {
        sherlock.heal(1);
        watson.heal(1);

        cout << "Fixed Point activated!\n";
        cout << "Watson is adjacent to Sherlock.\n";
        cout << "Sherlock and Watson gained 1 HP.\n";
    }
    else
    {
        cout << "Fixed Point failed: Watson is not adjacent to Sherlock.\n";
    }
}



// ---------------- Service ----------------

void ServiceEffect::apply(Fighter* attacker,Fighter* defender,Battle* battle,Card& card)
{

    Fighter& sherlock = battle->getSherlock();
    Fighter& watson = battle->getWatson();

    Zone* sherlockzone  = sherlock.getPosition();

    vector<Zone*> ok;
    for(Zone*z : sherlockzone->getNei())
    {
        if(battle->getfighterat(z) == nullptr)
        ok.push_back(z);
    }
    if(!ok.empty())
    {
        cout<<"Choose a zone adjacent to Sherlock\n";
    
    battle->getSherlock().heal(2);


    
    for(int i = 0; i < ok.size(); i++)
            cout << i + 1 << ") Zone " << ok[i]->getId() << endl;

            int choice;
        cin >> choice;
        choice--;
        
        if(choice >= 0 && choice < ok.size())
        {
            watson.setPosition(ok[choice]);
            
            cout << "Watson moved to Zone "
            << ok[choice]->getId() << endl;
        }
    }
    else
    {
        cout << "No empty adjacent zone for Watson.\n";
    }
    
    sherlock.heal(1);
    cout<<"Service: Sherlock healed 1 HP.\n";

      vector<Card> cards = attacker->getrandomcard(1);

        if(!cards.empty())
        {
            attacker->addtohand(cards);
            cout << "Drew 1 card.\n";
        }

    }




// ---------------- Study Method ----------------

void StudyMethodEffect::apply(Fighter* attacker,    Fighter* defender,    Battle* battle,    Card& card)
{
    if(attacker == nullptr || defender == nullptr)
        return;


    if(attacker->getName() != "Sherlock")
    {
        return;
    }


    int damage = battle->getFinalAttackValue() - battle->getFinalDefendValue();


    if(damage <= 0)
    {
        cout << "Sherlock did not win the battle.\n";
        return;
    }


    cout << "\nSherlock looks at enemy hand:\n";


    vector<Card>& enemyHand = defender->gethand();


    if(enemyHand.empty())
    {
        cout << "Enemy hand is empty.\n";
        return;
    }


    for(int i = 0; i < enemyHand.size(); i++)
    {
        cout << i + 1 << ") "
             << enemyHand[i].getName()
             << endl;
    }
}



// ---------------- Elementary ----------------

void ElementaryEffect::apply(Fighter* attacker,    Fighter* defender,    Battle* battle,    Card& card)
{

    int guess;

    cout<<"Elementary Prediction\n";
    cout<<"Guess attack value: ";
    cin>>guess;



    int realAttack =
        battle->getFinalAttackValue();



    if(guess == realAttack)
    {
        card.setValue(card.getValue()+2);

        cout<<"Correct prediction +2 defense\n";
    }
    else
    {
        cout<<"Wrong prediction\n";
    }

}



// ---------------- Impossible ----------------
void ImpossibleEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card)
{
    if(attacker == nullptr || defender == nullptr)
        return;


    int guess;

    cout << "Predict opponent attack value: ";
    cin >> guess;


    Card opponentAttack = battle->gelastattackcard();


    int realAttack = opponentAttack.getValue();


    if(guess == realAttack)
    {
        cout << "Correct prediction!\n";

        battle->setCancel(1);

        battle->setIgnoreAttack(true);
    }
    else
    {
        cout << "Wrong prediction!\n";
    }
}



// ---------------- Master Of Disguise ----------------

void MasterOfDisguiseEffect::apply(Fighter* attacker, Fighter* defender,  Battle* battle,  Card& card)
{
    Fighter& sherlock = battle->getSherlock();

    Fighter& dracula = battle->getDracual();

    Zone* sherlockZone = sherlock.getPosition();
    Zone* draculaZone = dracula.getPosition();

    sherlock.setPosition(draculaZone);
    dracula.setPosition(sherlockZone);

    dracula.takeDamage(1);

    cout << "Master of Disguise activated!\n";
    cout << "Sherlock and Dracula swapped positions.\n";
    cout << "Dracula takes 1 damage.\n";
}



// ---------------- Game On ----------------

void GameOnEffect::apply(Fighter* attacker,  Fighter* defender,   Battle* battle,   Card& card)
{
    if(attacker == nullptr || battle == nullptr)
        return;


    cout << "Game On Effect: Move 3 zones\n";


    vector<Zone*> reachable =  battle->getReachableZone(*attacker, 3);


    if(reachable.empty())
    {
        cout << "No available zone!\n";
        return;
    }


    cout << "Available zones:\n";

    for(int i = 0; i < reachable.size(); i++)
    {
        cout << i + 1 
             << ") Zone "
             << reachable[i]->getId()
             << endl;
    }


    int choice;
    cin >> choice;


    if(choice < 1 || choice > reachable.size())
    {
        cout << "Invalid choice\n";
        return;
    }


    Zone* targetZone = reachable[choice - 1];


    int distance = 0;

    for(auto z : reachable)
    {
        if(z == targetZone)
            break;

        distance++;
    }


    battle->movefighter( *attacker, targetZone->getId(), 3 );


    cout << attacker->getName() << " moved to zone " << targetZone->getId() << endl;
}


// ---------------- Confirm Suspicion ----------------

void ConfirmSuspicionEffect::apply(Fighter* attacker,
                                   Fighter* defender,
                                   Battle* battle,
                                   Card& card)
{

    if(!defender)
        return;


    cout<<"Confirm Suspicion activated.\n";


    if(defender->handsize()>0)
    {
        cout<<"Enemy has "
            <<defender->handsize()
            <<" cards.\n";
    }

}



// ---------------- Strategic Deduction ----------------

void StrategicDeductionEffect::apply( Fighter* attacker, Fighter* defender, Battle* battle, Card& card)
{
    if(defender == nullptr)
        return;

    vector<Card>& hand = defender->gethand();


    if(hand.empty())
        return;


    cout << "Choose opponent card to change boost:\n";


    for(int i = 0; i < hand.size(); i++)
    {
        cout << i+1 << ") "<< hand[i].getName()<< " Value: "<< hand[i].getValue()<< " Boost: "<< hand[i].getBoost()<< endl;
    }


    int choice;
    cin >> choice;


    if(choice < 1 || choice > hand.size())
        return;


    Card& enemyCard = hand[choice-1];


    int printedValue = enemyCard.getValue();

    enemyCard.setBoost(printedValue);


    cout << "Strategic Deduction activated!\n";
    cout << enemyCard.getName()
         << " boost changed to "
         << printedValue
         << endl;
}



// ---------------- Learning Never Ends ----------------

void LearningNeverEndsEffect::apply( Fighter* attacker, Fighter* defender, Battle* battle, Card& card)
{
    if(attacker == nullptr || defender == nullptr)
        return;


    int damage = battle->getFinalAttackValue()-battle->getFinalDefendValue();


    if(damage > 0)
    {
        cout << "Learning Never Ends: Opponent draws 1 card\n";
        defender->drawBoostMovement();
    }


  
    else
    {
        cout << "Learning Never Ends: You draw 2 cards\n";
        attacker->drawBoostMovement();
        attacker->drawBoostMovement();
    }
}



// ---------------- Deception ----------------

void DeceptionEffect::apply(Fighter* attacker,Fighter* defender,Battle* battle,Card& card)
{
    if(attacker == nullptr || defender == nullptr)
        return;

    vector<Card>& enemyHand = defender->gethand();

    if(enemyHand.empty())
    {
        cout << "Opponent has no cards to discard.\n";
        return;
    }


    cout << "\nOpponent hand:\n";

    for(int i = 0; i < enemyHand.size(); i++)
    {
        cout << i + 1 << ") "
             << enemyHand[i].getName()
             << endl;
    }


    int choice;

    while(true)
    {
        cout << "Choose a card to burn: ";
        cin >> choice;

        if(choice >= 1 && choice <= enemyHand.size())
            break;

        cout << "Invalid choice!\n";
    }


    cout << enemyHand[choice-1].getName() << " was burned.\n";


    enemyHand.erase(enemyHand.begin() + (choice - 1));
}



// ---------------- Sidearm ----------------

void SidearmEffect::apply(Fighter* attacker,
                          Fighter* defender,
                          Battle* battle,
                          Card& card)
{

    if(!defender)
        return;


    defender->takeDamage(
        card.getValue()
    );


    cout<<"Sidearm deals "
        <<card.getValue()
        <<" damage.\n";

}