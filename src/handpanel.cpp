#include "handpanel.h"
#include "card.h"
#include <iostream>


using namespace std;

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m"
#define BOLD    "\033[1m"
#define YELLOW   "\033[33m"

void printline(const string& title , const string& value)
{
      cout <<BOLD << YELLOW<< "| "<<BOLD<< CYAN << title <<RESET<< " : " << value<<RESET;

    int len = 2 + title.size() + 3 + value.size(); // "| " + title + " : " + value

    for (int i = len; i < 40; i++)
        cout << ' ';

    cout <<BOLD << YELLOW<< "|"<< RESET <<"\n";
}
void HandPanel::show(Fighter& fighter)
{
    vector<Card>& hand = fighter.gethand();

    cout << BOLD;

    if(fighter.isheroteam())
        cout << BLUE;
    else
        cout << RED;

    cout << "\n=============================================\n";
    cout << "                " << fighter.getName() << " HAND\n";
    cout << "=============================================\n";

    cout << RESET;

    const int wedth = 50;
    

    for(int i=0;i<hand.size();i++)
    {
        Card& card = hand[i];

        cout <<BOLD << YELLOW<< "-----------------------------------------"<< RESET <<"\n";

        cout << "| [" << i+1 << "] "
             << card.getName();

        for(int j=card.getName().size();j<28;j++)
            cout<<" ";

        cout<<BOLD << YELLOW<<"      |"<< RESET <<"\n";

        cout <<BOLD << YELLOW<<"-----------------------------------------"<< RESET <<"\n";

        printline("Type", card.cardtypetostring() );
        printline("Value", to_string(card.getValue() ));
        printline("Boost ", to_string(card.getBoost()) );
        printline("Owner", card.getowner() );
        printline("Time", card.timetostring() );

        cout <<BOLD << YELLOW<< "-----------------------------------------"<< RESET <<"\n";

        cout << "| Effect : "
             << card.getdescription() << '\n';

        cout <<BOLD << YELLOW<< "|---------------------------------------|"<< RESET <<"\n\n";
    }
}