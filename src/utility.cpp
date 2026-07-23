#include "battle.h"
#include <iostream>

using namespace std;

vector<Fighter>& Battle::getsisters()
{
    return sisters;
}

Map& Battle::getMap()
{
    return map;
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


void Battle::printfighters()
{
    cout << "\n----------------BASKERVILLE------------------\n\n";

    cout << "RED TEAM\n";

    cout
        << "DRACULA HP : "
        << dracula.getHealth()
        << " / PLACE : "
        << dracula.getPosition()->getId()
        << endl;

    for(int i = 0; i < sisters.size(); i++)
    {
        cout
            << "SISTER ("
            << i + 1
            << ") HP : "
            << sisters[i].getHealth()
            << " / PLACE : "
            << sisters[i].getPosition()->getId()
            << endl;
    }

    cout << "\nBLUE TEAM\n";

    cout
        << "SHERLOCK HP : "
        << sherlock.getHealth()
        << " / PLACE : "
        << sherlock.getPosition()->getId()
        << endl;

    cout
        << "WATSON HP : "
        << watson.getHealth()
        << " / PLACE : "
        << watson.getPosition()->getId()
        << endl;
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