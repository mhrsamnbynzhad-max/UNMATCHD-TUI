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
 vector<Fighter*> Dracula::getAbilityTargets(Battle* battle)
{
    vector<Fighter*> targets;
    Zone* currentZone = this->getPosition();
    if (currentZone == nullptr) return targets;

    for(Zone* neighbor : currentZone->getNei())
    {
        Fighter* occupant = battle->getfighterat(neighbor);
        if(occupant != nullptr)
        {
            targets.push_back(occupant);
        }
    }
    return targets;
}

void Dracula::useAbilityOn(Fighter* target)
{
    if(target == nullptr) return;
    target->takeDamage(1);
}

    void Dracula::specialAbillity(Battle* battle)
    {
        int choice = readInt("Use Dracula Ability? ( 1(Yes) / 0 (No) ): " , 0 ,1);
        if(choice != 1)
            return;

        vector<Fighter*> targets = getAbilityTargets(battle);

        if(targets.empty())
        {
            cout << "No valid targets.\n";
            return;
        }

        for(size_t i = 0; i < targets.size(); i++)
        {
            cout << i + 1 << ". " << targets[i]->getName() << endl;
        }

        int target = readInt("Choose Target: \n" , 1 , targets.size());

        if(target >= 1 && target <= targets.size())
        {
            useAbilityOn(targets[target - 1]);
        }
    }

void Dracula::setupUnits(Battle* battle, Player& player)
{
    battle->chooseSidekickPosition(player);
}


