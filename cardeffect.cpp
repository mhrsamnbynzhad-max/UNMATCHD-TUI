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
    //dibug
    battle->getsisters()[0].setPosition(battle->getMap().getZone(13));
    battle->getsisters()[1].setPosition(battle->getMap().getZone(11));
    battle->getsisters()[2].setPosition(battle->getMap().getZone(10));
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
//dibug
       cout<<"Available connected zones : \n";
       vector<Zone*> validmoves;
       cout<<"sisterposition";
       for(auto& s : battle->getsisters())
        cout<<s.getPosition()->getId()<<"   ";
        cout<<endl;
     
        cout<<"check fighter at zone 13";
        auto f = battle->getfighterat(battle->getMap().getZone(13));
        if(f)cout<<f->getName()<<endl;
        else cout<<"NULL"<<endl;
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

void FixedPointEffect::apply(Fighter* attacker,
                             Fighter* defender,
                             Battle* battle,
                             Card& card)
{
    if(!attacker)
        return;


    attacker->heal(3);

    cout<<"Fixed Point: "
        <<attacker->getName()
        <<" healed 3 HP.\n";
}



// ---------------- Service ----------------

void ServiceEffect::apply(Fighter* attacker,
                          Fighter* defender,
                          Battle* battle,
                          Card& card)
{

    battle->getSherlock().heal(2);


    cout<<"Service: Sherlock healed 2 HP.\n";
}



// ---------------- Study Method ----------------

void StudyMethodEffect::apply(Fighter* attacker,
                              Fighter* defender,
                              Battle* battle,
                              Card& card)
{
    if(!defender)
        return;


    cout<<"Study Method: "
        <<defender->getName()
        <<" hand:\n";


    vector<Card>& hand = defender->gethand();


    for(int i=0;i<hand.size();i++)
    {
        cout<<i+1<<") "
            <<hand[i].getName()
            <<" Boost: "
            <<hand[i].getBoost()
            <<endl;
    }

}



// ---------------- Elementary ----------------

void ElementaryEffect::apply(Fighter* attacker,
                             Fighter* defender,
                             Battle* battle,
                             Card& card)
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

void ImpossibleEffect::apply(Fighter* attacker,
                             Fighter* defender,
                             Battle* battle,
                             Card& card)
{

    if(!defender || defender->handsize()==0)
    {
        cout<<"Enemy has no cards\n";
        return;
    }


    Card removed =
        defender->remove_ranodmcard();


    cout<<"Impossible removed "
        <<removed.getName()
        <<endl;

}



// ---------------- Master Of Disguise ----------------

void MasterOfDisguiseEffect::apply(Fighter* attacker,
                                   Fighter* defender,
                                   Battle* battle,
                                   Card& card)
{

    Zone* sherlock =
        battle->getSherlock().getPosition();


    Zone* watson =
        battle->getWatson().getPosition();



    battle->getSherlock()
          .setPosition(watson);


    battle->getWatson()
          .setPosition(sherlock);



    cout<<"Master Of Disguise: Sherlock and Watson swapped.\n";

}



// ---------------- Game On ----------------

void GameOnEffect::apply(Fighter* attacker,
                         Fighter* defender,
                         Battle* battle,
                         Card& card)
{

    int zone;


    cout<<"Choose Sherlock destination: ";
    cin>>zone;



    if(battle->movefighter(
        battle->getSherlock(),
        zone,
        2))
    {
        cout<<"Sherlock moved.\n";
    }
    else
    {
        cout<<"Invalid movement.\n";
    }

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

void StrategicDeductionEffect::apply(Fighter* attacker,
                                     Fighter* defender,
                                     Battle* battle,
                                     Card& card)
{

    int boost =
        card.getBoost();


    card.setValue(
        card.getValue()+boost
    );


    cout<<"Strategic Deduction: +"
        <<boost
        <<" attack.\n";

}



// ---------------- Learning Never Ends ----------------

void LearningNeverEndsEffect::apply(Fighter* attacker,
                                    Fighter* defender,
                                    Battle* battle,
                                    Card& card)
{

    vector<Card> cards =
        attacker->getrandomcard(2);


    attacker->addtohand(cards);


    cout<<"Learning Never Ends: drew 2 cards.\n";

}



// ---------------- Deception ----------------

void DeceptionEffect::apply(Fighter* attacker,
                            Fighter* defender,
                            Battle* battle,
                            Card& card)
{

    battle->setCancel(true);


    cout<<"Deception: opponent effect cancelled.\n";

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