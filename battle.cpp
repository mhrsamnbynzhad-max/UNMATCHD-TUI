#include "battle.h"
#include "cardfactory.h"
#include <iostream>
#include <queue>

using namespace std;

    Battle::Battle():  sherlock(),  watson(),  dracula() , player1("Player 1") , player2("Player 2")
    {
        for(int i=0 ; i<3 ; i++)
        {
            sisters.push_back(
                Fighter("Sister",1,false,2 , false)
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
      
    }

    void Battle:: setuppositions()
    {
        sherlock.setPosition(map.getZone(1));
        
        showplacementzone(sherlock);
        int choice;
        cin>>choice;
          if(getfighterat(map.getZone(choice)))
            {
                cout<<"occuiped!\n";
            }
            else
            {
                    watson.setPosition(map.getZone(choice));
            } 

           dracula.setPosition(map.getZone(2));
          showplacementzone(dracula);
          for(int i = 0 ; i < 3 ; i++)
          {
            int choice;
            cin>>choice;  
            if(getfighterat(map.getZone(choice)))
            {
                cout<<"occuiped!\\n";
                i--;
                continue;
            }
            sisters[i].setPosition(map.getZone(choice));
         }

       
    }


void Battle::draculaability(Fighter* target)
{
    if(target == nullptr)
    {
        return;
    }

    target->takeDamage(1);

    cout << "Dracula damaged "
         << target->getName()
         << " for 1 damage\n";

    cout << "Dracula draws a card\n";
}

    void Battle:: printfighters()
    {
        cout<<"\n----------------BASKERVILLE------------------\n\n";
        cout<<"RED TEAM \n";
       cout<< "DRACULA HP : "<<dracula.getHealth()<<" / PLACE : "<<dracula.getPosition()->getId()<<endl;
       for(int i  = 0 ; i < sisters.size() ; i ++)
       {
           cout<<"SISTER "<< "( "<<i + 1<<" ) "<<"HP  :"<<sisters[i].getHealth()<<" / PLACE : "<<sisters[i].getPosition()->getId()<<endl;

       }
        cout<<"\nBLUE TEAM\n ";
      
       cout<<"SHERLOCK HP : "<<sherlock.getHealth()<<" / PLACE : "<< sherlock.getPosition()->getId()<<endl;
       if(sherlock.getPosition() != nullptr)
       {
        cout<<"Zone"<<sherlock.getPosition()->getId();
       }
       cout<<endl;

        cout<< watson.getName()<<"HP"<<watson.getHealth()<<endl;
       if(watson.getPosition() != nullptr)
       {
        cout<<"Zone"<<watson.getPosition()->getId();
       }
       cout<<endl;


      
    }


    void Battle :: combat(Fighter* attracker,Fighter* defender )
    {

        if(attracker->handsize() == 0)
        {
            cout<<"Atracker has no cards";
            return;
        }
         if(defender->handsize() == 0)
        {
            cout<<"Defender has no cards";
            return;
        }

        Card attackcard = attracker->playcard(0);
        Card defendcard ;

        bool isdefended = false;
        cout<<defender->getName()<<"  Do yo want to defend? (yes(1) or no (0))\n";
        int choose;
        cin>>choose;

         if(choose == 1 && defender->handsize()>0)
         {
            isdefended = true;
            defendcard = (*defender).playcard(0);
         }
         else
         {
            isdefended = false;
            defendcard;
         }
         int attackValue = attackcard.getValue();

         int defederValue = isdefended ? defendcard.getValue() : 0;

         int damage = attackValue - defederValue;

        if (attackcard.getcardname() == BLOOD_THIRST)
            {
                Zone* enemyZone = defender->getPosition();          // Place of defender
                char color = enemyZone->getColor();                 // Color of defender place

                vector<Zone*> sameColorZones = map.getZonebycolor(color);   // All places that is the same color 

                int counter = 0;

                for (const Fighter& sis : sisters)                 
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

                attackValue += counter;                             // Increases attack 
            }
            cout<<"Attacker played :"<<attackcard.getName()<<endl;
            cout<<"Defender played :"<<defendcard.getName()<<endl;

            damage =  attackValue - defendcard.getValue();
            if(damage >0 )
            {
                defender->takeDamage(damage);

                cout<<"Damage"<<damage<<endl;
            }


    }

  
 void Battle::applycardeffect(Card card)
  {
     switch (card.getcardname())
     {  
         case GAME_ON:
        cout<<"Move holmes 3 spaces"<<endl;
        break;

         case MASTER_OF_DISGUISE:
        cout<<"Swap holems and enemy"<<endl;
        break;


          case IMPOSSIBLE:
        cout<<"Look at enemy hand"<<endl;
        break;

        case SERVICE:
        cout<<"Holmes healed"<<endl;
        break;
        
         case FIXED_POINT:
        cout<<"Watson and  Holmes healed"<<endl;
        break;

         case COUNTER_ATTACK:
        cout<<"Deal 2 extra damage"<<endl;
        break;

         case STUDY_METHOD:
        cout<<"See enemy hand"<<endl;
        break;

         case LEARNING_NEVER_ENDS:
        cout<<"DRAW cards depending on result"<<endl;
        break;
      
         case STRATEGIC_DEDUCTION:
        cout<<"Replace value with boost"<<endl;
        break;

         case ELEMENTARY:
        cout<<"Prediction defends"<<endl;
        break;

         case CONFIRM_SUSPICION:
        cout<<"Guess enemy value"<<endl;
        break;
    
     }
  
  }


    bool Battle :: areadjacent(Fighter& a ,Fighter& b )
    {
        for(auto n : a.getPosition()->getNei() )
        {
            if(n == b.getPosition())
             return true;
          
        }
        if(map.issecretzone(a.getPosition()->getId())&& map.issecretzone(b.getPosition()->getId()) && a.getPosition() != b.getPosition())
        {
            return true; 
        }

        return false;

    }

    bool  Battle :: canreach(Zone* current , Zone* target , int movesleft  , Fighter& mover)
    {
        if(current == target)
        {
            return true ;
        }
         if(movesleft<= 0)
        {
            return false ;
        }

        for(Zone* next : current->getNei())
        {
               Fighter* occupant  = getfighterat(next);
              if(occupant != nullptr)
              {
                bool ally = occupant->isheroteam() == mover.isheroteam();

                if(!ally)
                {
                    continue;
                }
              }

            if(canreach(next ,target , movesleft -1 , mover))
            {
                return true;
            }
        }

        if(map.issecretzone(current->getId()))
        {
            const vector<int>& secret = map.getsecretZones();
            for(int  i = 0 ; i <secret.size() ; i++)
            {
                    if(secret[i] == current->getId())
                    {
                         continue;
                    }
                    if(canreach(map.getZone(secret[i]), target ,movesleft-1 , mover))
                    {
                        return true;
                    }
                
            }
        }
        return false;
         
    }


    bool  Battle ::  movefighter(Fighter& fighter ,int destinationid , int Maxmove)
    {

        showPossiblemoves(fighter);
      Zone* destination = map.getZone(destinationid);

      if(destination == fighter.getPosition())
      {
        return true ;
      }
      
      if(destination == nullptr)
      {
        return false ;
      }  

      if(!canreach(fighter.getPosition(), destination , Maxmove , fighter))
      {
        return false;
      }
        Fighter* occupant  = getfighterat(destination);
        if(occupant != nullptr)
        {
            cout<<"Can't stop there"<<endl;
            return false ;
        }
        fighter.setPosition(destination);
        return true;
      
    }

    void Battle :: showPossiblemoves(Fighter&  fight)
    {
        Zone* now = fight.getPosition();
        cout<<"Possible Moves : \n";
        for(Zone* n : now->getNei())
        {
            cout<<n->getId()<<" ";
        }
        cout<<endl;
    }
   Fighter* Battle::  getfighterat(Zone* zone)
   {
          if(dracula.getPosition() == zone)
       return& dracula ;

        if(sherlock.getPosition() == zone)
       return &sherlock ;

        if(watson.getPosition() == zone)
       return &watson ;

       for (int i = 0 ; i < sisters.size() ; i ++)
       {
           if(sisters[i].getPosition() == zone)
           {
               return &sisters[i];
           }
       }

       return nullptr; 
   }


    void  Battle :: showplacementzone(Fighter& hero )
    {
         vector<Zone*>zones = map.getplacementZone(hero.getPosition());
         cout<<"\nAvailable zones";


         for(int i = 0 ; i <zones.size() ; i ++)
         {
            if(getfighterat(zones[i] )== nullptr)
            {
                cout<<zones[i]->getId()<<" ";    

            }
         }
         cout<<endl;
    }
    
    void Battle::startGame()
{
    std::queue<Player*> turnQueue;
    turnQueue.push(&player1);
    turnQueue.push(&player2);

    while(true)
    {
        Player* current = turnQueue.front();
        turnQueue.pop();

        cout << "\nTurn : " << current->getName() << endl;
        cout << "1) Draw\n2) Maneuver\n3) Attack\n4) Scheme\n";

        int choice;
        cin >> choice;

        Player* enemy = (current == &player1 ? &player2 : &player1);

        try
        {
            switch(choice)
            {
                case 1: current->drawCard(); break;
                case 2: current->maneuver(*this); break;
                case 3: current->attack(*enemy, *this); break;
                case 4: current->playScheme(*enemy, *this); break;
                default: throw invalid_argument("Invalid input...");
            }
        }
        catch(const exception& e)
        {
            cout << "Error: " << e.what() << endl;
        }

        turnQueue.push(current);
    }
}