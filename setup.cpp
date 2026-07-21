#include "battle.h"
#include <iostream>

using namespace std;

void Battle::setuppositions()
{
    sherlock.setPosition(map.getZone(18));

    showplacementzone(sherlock);

    int choice;
    cin >> choice;

    if(getfighterat(map.getZone(choice)))
    {
        cout << "occupied!\n";
    }
    else
    {
        watson.setPosition(map.getZone(choice));
    }

    dracula.setPosition(map.getZone(19));

    showplacementzone(dracula);

    for(int i = 0; i < 3; i++)
    {
        int choice;
        cin >> choice;

        if(getfighterat(map.getZone(choice)))
        {
            cout << "occupied!\n";
            i--;
            continue;
        }

        sisters[i].setPosition(map.getZone(choice));
    }
}

void Battle::showplacementzone(Fighter& hero)
{
    vector<Zone*> zones =
        map.getplacementZone(hero.getPosition());

    cout << "\nAvailable zones: ";

    for(int i = 0; i < zones.size(); i++)
    {
        if(getfighterat(zones[i]) == nullptr)
        {
            cout << zones[i]->getId() << " ";
        }
    }

    cout << endl;
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