#include "battle.h"
#include "cardfactory.h"
#include "cardeffect.h"
#include <iostream>
#include <queue>

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

     ZoneCheckResult  Battle:: canEnterzone(Fighter* mover , Fighter* occupant ,int moveleft )
     {
        ZoneCheckResult result;
        result.allow = true;
        if(!occupant)
         return result;

        string name = occupant ->getName();

         if(name == "Sherlock" || name == "Watson")
         {
            result.allow = false;
            result.blocker = name;
            return result;
         }

         if(name == "Sister")
         {
            if(moveleft == 0)
            {
                 result.allow = false;
                 result.blocker = "Sister";
                 return result;
            }
             result.allow = true;
             result.blocker = "Sister";
             return result;
         }

         return result;
     }
    void Battle:: setuppositions()
    {
        sherlock.setPosition(map.getZone(18));
        
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

            dracula.setPosition(map.getZone(19));
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
        
        
    vector<Fighter>& Battle:: getsisters()
    {
    return sisters;
    }
    Map& Battle::  getMap()
    {
        return map;
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

    


    void Battle :: combat(Fighter* attracker,Fighter* defender, int cardindex)
    {
        ignoreAttackValue = false;
        ignoreDefendValue = false;

        finalAttackValue = 0;
        finalDefendValue = 0;

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

        Card  attackcard = attracker->playcard(cardindex);
        this->lastAttackCard = attackcard;
        Card defendcard ;

        bool isdefended = false;
        cout<<defender->getName()<<"  Do yo want to defend? (yes(1) or no (0))\n";
        int choose;
        cin>>choose;

         if(choose == 1 && defender->handsize()>0)
         {
            isdefended = true;
            vector<int> defenseIndexes;

            for(int i = 0; i < defender->handsize(); i++)
            {
               Cardtype type = defender->gethand()[i].getcardType();

                if(type == DEFENSE || type == VERSATILE)
                {
                    defenseIndexes.push_back(i);
                }
            }

            if(defenseIndexes.empty())
            {
                cout << defender->getName() << " has no defense cards.\n";
                isdefended = false;
                defendcard = Card(); // NO_CARD
            }
            else
            {
                cout << "Choose a defense card:\n";
                for(int i = 0; i < defenseIndexes.size(); i++)
                {
                    int idx = defenseIndexes[i];

                    cout << i + 1 << ") "
                        << defender->gethand()[idx].getName()
                        << " (DEF " << defender->gethand()[idx].getValue() << ")\n";
                }

                int choice;
                cin >> choice;
                choice--;

                if(choice < 0 || choice >= defenseIndexes.size())
                {
                    cout << "Invalid choice.\n";
                    isdefended = false;
                    defendcard = Card();
                }
                else
                {
                    int realIndex = defenseIndexes[choice];
                    defendcard = defender->playcard(realIndex);
                    isdefended = true;
                }
                        
            }
            
         }
         else
         {
            isdefended = false;
            defendcard;
         }
        if(isdefended &&
            defendcard.getTiming() == BEFOR_COMBAT)
            {
                if(!this->getCancel())
                {
                    applycardeffect(defendcard , defender ,attracker);
                }

            else
            {
               
               cout<<"Opponent card's effect is cancelled\n";
            }

         }
         
    finalAttackValue = attackcard.getValue();
    finalDefendValue = isdefended ? defendcard.getValue() : 0;

    if(ignoreAttackValue)
    {
        finalAttackValue = 0;
    }

    if(ignoreDefendValue)
    {
        finalDefendValue = 0;
    }
    if(attackcard.getTiming() == BEFOR_COMBAT)
    {
        applycardeffect(attackcard ,attracker ,defender);
    }
    if(isdefended &&
        defendcard.getTiming() == DURING_COMBAT)
        {
            applycardeffect(defendcard , defender ,attracker);
        }

        if(attackcard.getTiming() == DURING_COMBAT)
        {
            applycardeffect(attackcard ,attracker ,defender);
        }
    lastFinaldefend = finalDefendValue;

    int damage = finalAttackValue - finalDefendValue;
         
         cout<<"Attacker played :"<<attackcard.getName()<<endl;
         cout<<"Defender played :"<<defendcard.getName()<<endl;
            
    if(damage >= 0 )
        {
            defender->takeDamage(damage);

            cout<<"Damage ( "<<damage<<" )" <<endl;
        }

        if(isdefended &&
        defendcard.getTiming() == AFTER_COMBAT)
        {
            applycardeffect(defendcard , defender ,attracker);
        }

        if(attackcard.getTiming() == AFTER_COMBAT)
        {
            applycardeffect(attackcard ,attracker ,defender);
        }

    this->setCancel(false);
    }


    void Battle :: applycardeffect(Card& card , Fighter* attacker ,Fighter* defender)
    {
           CardEffect* effect = card.getEffect();
           if(effect)
           {
              effect->apply(attacker,defender,this,card);
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

    vector<Zone*> Battle :: getReachableZone ( Fighter&  fighter, int maxMove)
    {
        vector<Zone*> result;

    for(int i = 1; i <= 32; i++)
    {
        Zone* z = map.getZone(i);

        if(z == fighter.getPosition())
            continue;

        if(canreach(fighter.getPosition(), z, maxMove, fighter))
        {
            if(getfighterat(z) == nullptr)
                result.push_back(z);
        }
    }

    return result;

    }
    
    bool  Battle ::  movefighter(Fighter& fighter ,int destinationid , int Maxmove)
    {
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
    