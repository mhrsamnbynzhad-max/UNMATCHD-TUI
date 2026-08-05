#include "dracula.h"
#include "battle.h"
#include "fighter.h"
#include "handling.h"
#include <iostream>
#include <vector>

using namespace std;

Dracula::Dracula() : Fighter("Dracula",13,13,false,2 , DRACULA)
{
}

void Dracula:: specialabilityattack(Fighter* target)
{
    if(target == nullptr)
        return;

    target->takeDamage(1);

    cout<<"Dracula damaged " <<target->getName()<<endl;
}
void Dracula:: specialAbillity(Battle* battle)
{
        int choice = readInt("Use Dracula Ability? ( 1(Yes) / 0 (No) ): " , 0 ,1);

    if(choice != 1 )
      return;

    Zone* currentZone = this->getPosition();

    vector<Fighter*> targets;

    for(Zone* neighbor : currentZone->getNei())
    {
        Fighter& sherlock = battle->getSherlock();
        if(sherlock.isalive() &&
           sherlock.getPosition() == neighbor)
        {
            targets.push_back(&sherlock);
        }

        Fighter& watson = battle->getWatson();

        if(watson.isalive() &&
           watson.getPosition() == neighbor)
        {
            targets.push_back(&watson);
        }
        vector<Sisters>& sisters = battle->getsisters();

        for(Fighter& sister : sisters)
        {
            if(sister.isalive() &&
            sister.getPosition() == neighbor)
            {
                targets.push_back(&sister);
            }
        }
    }

    if(targets.empty())
    {
        cout << "No valid targets.\n";
        return;
    }


    for(size_t i = 0; i < targets.size(); i++)
    {
        cout << i + 1<< ". " << targets[i]->getName() << endl;
    }

    int target = readInt("Choose Target: \n" , 1 , targets.size() );
    
    if(target >= 1 && target <= targets.size())
    {
        targets[target - 1]->takeDamage(1);
    }
}
