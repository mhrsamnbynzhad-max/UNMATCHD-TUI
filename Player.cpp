#include "player.h"
#include "battle.h"
#include <iostream>


using namespace std;

Player::Player(const string& n) : name(n), hero(nullptr)
{
}

string Player::getName() const
{
    return name;
}

Fighter* Player::getHero() const
{
    return hero;
}

void Player::chooseHero(Fighter* sherlock, Fighter* dracula)
{
    int choice;

    while (true)
    {
        cout << "\n" << name << " Choose your FIGHTER:\n";
        cout << "1) Sherlock\n";
        cout << "2) Dracula\n";
        cout << "Enter choice: ";

        cin >> choice;

        if (!cin)
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input! Try again.\n";
            continue;
        }

        if (choice == 1)
        {
            hero = sherlock;
            break;
        }
        else if (choice == 2)
        {
            hero = dracula;
            break;
        }
        else
        {
            cout << "Invalid selection, must be 1 or 2.\n";
        }
    }

    cout << name << " selected " << hero->getName() << "!\n";
}
void Player::drawCard()
{
    if(hero == nullptr)
        throw logic_error("The player HAS NOT SELECTED a fighter yet..!");

        auto cards = hero->getrandomcard(5);

        for(auto& c : cards)
        {
            hero->addtohand(cards);
        }

    cout << name << " DRAW 5 CARDs...\n";

    
}

void Player::maneuver(Battle& battle)
{
    if(hero == nullptr)
        throw logic_error("The player HAS NOT SELECTED a fighter yet..!");

        auto cards = hero->getrandomcard(1);
        
    if(!cards.empty())
    {
        hero->addtohand(cards);
        cout<<"Draw a card do to manemuver\n";
    }

    cout<<"deck card after maneuver.."<<hero->getdecksize()<<endl;
    cout << "Enter a destination....";
    int dest;
    cin >> dest;

    if(!battle.movefighter(*hero, dest, hero->getmovement()))
        throw runtime_error("Invalid move ....");

    cout << hero->getName() << " Move completed.\n";
}

void Player::attack(Player& enemy, Battle& battle)
{
    if(hero == nullptr || enemy.getHero() == nullptr)
        throw logic_error("No fighter selected ..");

    cout << hero->getName() << "ATTACKS... " 
         << enemy.getHero()->getName() << endl;

    battle.combat(hero, enemy.getHero());
}

void Player::playScheme(Player& enemy, Battle& battle)
{
    cout << "Card execuation Scheme (Development..)\n";
}

    void Player:: setHero(Fighter*  h )
    {
              hero = h ;
    }
