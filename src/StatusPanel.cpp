#include "StatusPanel.h"
#include "battle.h"
#include <iostream>
using namespace std;

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define BOLD "\033[1m"

static void printHP(int hp, int maxhp) {
    double ratio = static_cast<double>(hp) / maxhp;
    if(ratio > 0.6) cout << GREEN;
    else if(ratio > 0.3) cout << YELLOW;
    else cout << RED;
    cout << hp << RESET;
}

void StatusPanel::show(Battle& battle) {
    cout << BOLD << CYAN << "\n==================================================\n" 
         << " UNMATCHED STATUS\n" 
         << "==================================================\n" 
         << RESET;

    vector<Fighter*> activeFighters = battle.getFighters();

    for(Fighter* f : activeFighters) {
        if(f == nullptr || !f->isalive() || f->getPosition() == nullptr) continue;

        cout << BOLD << YELLOW << f->getName() << RESET << " | HP: ";
        printHP(f->getHealth(), f->getMaxealth());
        cout << " | Hand: " << f->handsize() << " | Zone: " << f->getPosition()->getId() << "\n";
    }

    cout << CYAN << "==================================================\n" << RESET;
}