#include "battle.h"
#include <vector>
#include<iostream>
#include <cstdlib>
#include <queue>
#include <set>


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
                bool ally = occupant->getteam() == mover.getteam();

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
         // Fog token movement (Invisible Man)
        if(mover.getteam() == INVISIBLE)
        {
            bool onFog = false;

            for(FogToken& fog : fogtoken)
            {
                if(fog.getPosition() == current)
                {
                    onFog = true;
                    break;
                }
            }

            if(onFog)
            {
                for(FogToken& fog : fogtoken)
                {
                    Zone* next = fog.getPosition();

                    if(next == current)
                        continue;

                    Fighter* occupant = getfighterat(next);

                    if(occupant != nullptr && occupant->getteam() != mover.getteam())
                        continue;

                    if(canreach(next, target, movesleft - 1, mover))
                        return true;
                }
            }
        }
        if(map.issecretzone(current->getId()))
        {
            const vector<int>& secret = map.getsecretZones();

            for(int i = 0; i < secret.size(); i++)
            {
                if(secret[i] == current->getId())
                    continue;

                Zone* next = map.getZone(secret[i]);

                Fighter* occ = getfighterat(next);

                if(occ)
                {
                    bool ally = occ->getteam() == mover.getteam();

                    if(!ally)
                        continue; 
                }

                if(canreach(next, target, movesleft - 1, mover))
                    return true;
            }
        }


        return false;
         
    }


    vector<Zone*> Battle :: getReachableZone ( Fighter&  fighter, int maxMove)
    {
        vector<Zone*> result;

        result.push_back(fighter.getPosition());

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

    bool Battle::moveFogToken(int fogIndex, int destinationId)
{
    if(fogIndex < 0 || fogIndex >= fogtoken.size())
        return false;


    Zone* destination = map.getZone(destinationId);

    if(destination == nullptr)
        return false;


    for(int i = 0; i < fogtoken.size(); i++)
    {
        if(i != fogIndex && fogtoken[i].getPosition() == destination)
        {
            cout << "There is already a Fog Token here\n";
            return false;
        }
    }


    fogtoken[fogIndex].setPosition(destination);

    return true;
}

vector<Zone*> Battle::getReachableZoneFromZone(Zone* start,int maxMove)
{
    vector<Zone*> result;

    queue<pair<Zone*,int>> q;

    set<Zone*> visited;


    q.push({start,0});
    visited.insert(start);


    while(!q.empty())
    {
        Zone* current = q.front().first;
        int dist = q.front().second;

        q.pop();


        if(dist == maxMove)
            continue;


        for(Zone* next : current->getNei())
        {
            if(visited.count(next))
                continue;


            visited.insert(next);

            result.push_back(next);

            q.push({next,dist+1});
        }
    }


    return result;
}