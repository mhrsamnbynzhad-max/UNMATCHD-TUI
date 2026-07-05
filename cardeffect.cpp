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

        cout<<"Ambush : oppenet discarded "<<remove.getName()<<" and + " <<opponentBoost <<" attack added . \n";

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
