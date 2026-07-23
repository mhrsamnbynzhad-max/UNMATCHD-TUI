#include "dracula.h"
#include <iostream>

using namespace std;

Dracula::Dracula() : Fighter("Dracula",13,13,false,2 , false)
{
}

void Dracula::specialAbility(Fighter* target)
{
    if(target == nullptr)
        return;

    target->takeDamage(1);

    cout<<"Dracula damaged " <<target->getName()<<endl;
}