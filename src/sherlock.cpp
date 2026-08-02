#include "sherlock.h"
#include "handling.h"
#include "battle.h"
#include<iostream>

using namespace std;


Sherlock::Sherlock()  :Fighter("Sherlock", 16 , 16 ,true,2 , SHERLOCK)
{
    
}
void Sherlock:: specialAbillity(Battle* battle)
{
    int choice = readInt("Use Sherlock Ability? ( 1 (Yes)/ 2 (No): " , 1 , 2);

    if(choice == 1)
    {
        battle->setSherlockability(true);

        cout << "Sherlock ability activated.\n";
    }
}