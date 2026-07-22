#include "battle.h"
#include <vector>
#include<iostream>

using namespace std;

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
  