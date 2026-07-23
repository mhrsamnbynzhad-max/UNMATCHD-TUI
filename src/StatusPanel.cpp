#include "StatusPanel.h"
#include "battle.h"
#include <iostream>

using namespace std;

#define RESET   "\033[0m"

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"

#define BOLD    "\033[1m"

static void printHP(int hp, int maxhp)
{
    double ratio =
        static_cast<double>(hp) / maxhp;

    if(ratio > 0.6)
        cout << GREEN;
    else if(ratio > 0.3)
        cout << YELLOW;
    else
        cout << RED;

    cout << hp << RESET;
}

void StatusPanel::show(Battle& battle)
{
    Fighter& sherlock = battle.getSherlock();
    Fighter& watson   = battle.getWatson();
    Fighter& dracula  = battle.getDracual();

    cout
        << BOLD
        << CYAN
        << "\n==================================================\n"
        << "                 UNMATCHED STATUS\n"
        << "==================================================\n"
        << RESET;

    cout
        << BLUE
        << "\nBLUE TEAM\n"
        << RESET;

    cout << "Sherlock | HP: ";
    printHP(
        sherlock.getHealth(),
        sherlock.getMaxealth()
    );

    cout
        << " | Hand: "
        << sherlock.handsize()
        << " | Zone: "
        << sherlock.getPosition()->getId()
        << "\n";

    cout << "Watson   | HP: ";

    printHP(
        watson.getHealth(),
        watson.getMaxealth()
    );

    cout
        << " | Hand: "
        << watson.handsize()
        << " | Zone: "
        << watson.getPosition()->getId()
        << "\n";

    cout
        << RED
        << "\nRED TEAM\n"
        << RESET;

    cout << "Dracula  | HP: ";

    printHP(
        dracula.getHealth(),
        dracula.getMaxealth()
    );

    cout
        << " | Hand: "
        << dracula.handsize()
        << " | Zone: "
        << dracula.getPosition()->getId()
        << "\n";

    vector<Fighter>& sisters =
        battle.getsisters();

    for(size_t i = 0; i < sisters.size(); i++)
    {
        cout
            << "Sister "
            << i + 1
            << " | HP: ";

        printHP(
            sisters[i].getHealth(),
            sisters[i].getMaxealth()
        );

        cout
            << " | Zone: "
            << sisters[i].getPosition()->getId()
            << "\n";
    }

    cout
        << CYAN
        << "\n==================================================\n"
        << RESET;
}