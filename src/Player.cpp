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

        hero->addtohand(cards);
  
    
}

void Player::maneuver(Battle& battle)
{
    if(hero == nullptr)
        throw logic_error("The player HAS NOT SELECTED a fighter yet..!");

        Card drawn = hero->drawBoostMovement();

        cout << "\nA card was drawn and added to your hand.\n";

        vector<Card>& hand = hero->gethand();

        for(int i=0;i<hand.size();i++)
        {
            cout
                << i+1
                << ") "
                << hand[i].getName()
                << " (Boost = "
                << hand[i].getBoost()
                << ")\n";
        }

   cout << "Choose a card to use its Boost (0 = No Boost): ";

        int choice;
        cin >> choice;

        int boost = 0;

        if(choice != 0)
        {
            choice--;

            if(choice < 0 || choice >= hand.size())
                throw runtime_error("Invalid card.");

            boost = hand[choice].getBoost();

            cout << "Using "<< hand[choice].getName()<< " (Boost = "<< boost<< ")\n";
        }
        else
        {
            cout << "No Boost selected.\n";
        }

        int movemax = hero->getmovement() + boost;

        cout << "Maximum movement = "<< movemax << endl;
    
         vector<Fighter*> movable;

        if(hero->getName()=="Sherlock")
        {
            movable.push_back(hero);
            movable.push_back(&battle.getWatson());
        }
        else
        {
            movable.push_back(hero);

            vector<Fighter>& sisters = battle.getsisters();

            for(auto& s : sisters)
                movable.push_back(&s);
        }

        vector<bool> moved(movable.size(),false);

        int movedCount=0;

        while(movedCount < movable.size())
        {
            cout<<"\nChoose fighter to move ( 0 end):\n";

            for(int i=0;i<movable.size();i++)
            {
                if(moved[i])
                    continue;

                cout<<i+1<<") "
                    <<movable[i]->getName();

                if(movable[i]->getName()=="Sister")
                    cout<<" "<<i;

                cout<<endl;
            }

            int choice;
            cin>>choice;

            if(choice==0)
                break;

            choice--;

            if(choice<0 || choice>=movable.size())
                continue;

            if(moved[choice])
            {
                cout<<"Already moved.\n";
                continue;
            }

            Fighter* selected = movable[choice];

            vector<Zone*> moves =
                battle.getReachableZone(*selected,movemax);

            cout<<"Possible destinations:\n";

            for(auto z:moves)
                cout<<z->getId()<<" ";

            cout<<endl;

            cout<<"Destination: ";

            int dest;
            cin>>dest;

            if(battle.movefighter(*selected,dest,movemax))
            {
                moved[choice]=true;
                movedCount++;
            }
            else
            {
                cout<<"Invalid move.\n";
            }
        }
}

void Player::attack(Player& enemy, Battle& battle,Fighter* attacker , int cardindex)
{
    if(hero == nullptr || enemy.getHero() == nullptr)
        throw logic_error("No fighter selected ..");

    cout << attacker->getName() << "ATTACKS... " 
         << enemy.getHero()->getName() << endl;

    battle.combat(attacker, enemy.getHero() , cardindex);
}

   void Player::playScheme(Player& enemy, Battle& battle, int cardindex)
    {
        Card schemecard = hero->playcard(cardindex);

        battle.applycardeffect(schemecard,hero,enemy.getHero());

        cout << "Card execution Scheme\n";
    }
    void Player:: setHero(Fighter*  h )
    {
              hero = h ;
    }

    Fighter* Player:: chooseAttacker( Battle& battle )
    {
         Fighter* hero = getHero();

    if(hero->getName() == "Sherlock")
    {
            cout<<"Attack with:\n";
            cout<<"1) Sherlock\n";
            cout<<"2) Watson\n";

            int ch;
            cin>>ch;

            if(ch==2)
                return &battle.getWatson();

            return hero;
        }

        if(hero->getName()=="Dracula")
        {
            cout<<"Attack with:\n";
            cout<<"1) Dracula\n";
            cout<<"2) Sister 1\n";
            cout<<"3) Sister 2\n";
            cout<<"4) Sister 3\n";

            int ch;
            cin>>ch;

            if(ch>=2 && ch<=4)
                return &battle.getsisters()[ch-2];

            return hero;
        }

        return hero;
    }

    bool Player::chooseAttackerIfNeeded(Battle& battle,Card& card,Fighter*& attacker)
{
    if(card.getcardType()==SCHEME)
    {
        attacker = nullptr;
        return true;
    }

    attacker = chooseAttacker(battle);

    return true;
}

