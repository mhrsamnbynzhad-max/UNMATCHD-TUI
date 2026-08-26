#include "battle.h"
#include "combatmanager.h"
#include "boardmanager.h"
#include "cardfactory.h"
#include "handling.h"
#include "map.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

Battle::Battle() 
    : sherlock(), watson(), dracula(), sisters(), invisibleman(), fogtoken(), 
      player1("Player 1"), player2("Player 2")
{
    combatManager = new CombatManager(this);
    boardManager = new BoardManager(this);

    srand(time(0));
    for(int i = 0; i < 3; i++)
    {
        sisters.emplace_back();
        fogtoken.push_back(FogToken());
    }
     availableHeroes = { &sherlock, &dracula, &invisibleman };

}

Battle::~Battle()
{
    delete combatManager;
    delete boardManager;
}

  void Battle::setAges(int age1, int age2)
{
    if(age1 < age2) playerfirst = true;
    else if(age2 < age1) playerfirst = false;
    else playerfirst = rand() % 2;
}

Player& Battle::getFirstChooser()  { return playerfirst ? player1 : player2; }
Player& Battle::getSecondChooser() { return playerfirst ? player2 : player1; }

std::vector<Fighter*>& Battle::getAvailableHeroes() { return availableHeroes; }

void Battle::assignHero(Player& player, Fighter* hero)
{
    player.setHero(hero); 
    availableHeroes.erase(remove(availableHeroes.begin(), availableHeroes.end(), hero), availableHeroes.end());
}

void Battle::beginUnitSetup()
{
    for (Fighter* unchosen : availableHeroes) 
    {
        unchosen->setPosition(nullptr);
        unchosen->sethealth(0);
    }
    setuppositions();
}

vector<int> Battle::getSidekickValidZones(Player& player) {
    Fighter* hero = player.getHero();
    if (hero->getName() == "Sherlock") return boardManager->getPlacementZoneIds(sherlock);
    if (hero->getName() == "Dracula")  return boardManager->getPlacementZoneIds(dracula);
    if (hero->getName() == "InvisibleMan") {
        vector<Zone*> zones = map.getplacementZone(hero->getPosition());
        vector<int> ids;
        for(Zone* z : zones) {
            bool used = false;
            for(FogToken& fog : fogtoken) {
                if(fog.getPosition() == z) { used = true; break; }
            }
            if(!used) ids.push_back(z->getId());
        }
        return ids;
    }
    return {};
}

bool Battle::placeSidekickAt(Player& player, int zoneId) {
    Fighter* hero = player.getHero();
    Zone* zone = map.getZone(zoneId);
    if (zone == nullptr) return false;

    if (hero->getName() == "Sherlock") {
        if (getfighterat(zone) != nullptr) return false;
        watson.setPosition(zone);
        return true;
    } else if (hero->getName() == "Dracula") {
        if (getfighterat(zone) != nullptr) return false;
        sisters[sidekickIndex].setPosition(zone);
        sidekickIndex++;
        if (sidekickIndex >= 3) { sidekickIndex = 0; return true; }
        return false;
    } else if (hero->getName() == "InvisibleMan") {
        for(FogToken& fog : fogtoken) {
            if(fog.getPosition() == zone) return false; // این خونه از قبل فوگ داره
        }
        fogtoken[fogIndex].setPosition(zone);
        fogIndex++;
        if (fogIndex >= 3) { fogIndex = 0; return true; }
        return false;
    }
    return true;
}

    void Battle::finalizeSetup()
    {
        dracula.setdeck(CardFactory::createDraculaDeck());
        sherlock.setdeck(CardFactory::createSherlockDeck());
        invisibleman.setdeck(CardFactory::createInvisibleDeck());
        player1.drawCard();
        player2.drawCard();
    }

    void Battle::startGame()
    {
        if (playerfirst) { turnQueue.push(&player1); turnQueue.push(&player2); }
        else { turnQueue.push(&player2); turnQueue.push(&player1); }
        actionsThisTurn = 0;
        startTurn(*turnQueue.front());
    }

    Player& Battle::getCurrentPlayer() { return *turnQueue.front(); }

    Player& Battle::getOtherPlayer(Player& p) { return (&p == &player1) ? player2 : player1; }

    void Battle::endTurnAndAdvance()
    {
        Player* current = turnQueue.front();
        turnQueue.pop();

        vector<Card>& hand = current->getHero()->gethand();
        while (hand.size() > 7) hand.pop_back();

        turnQueue.push(current);
        actionsThisTurn = 0;
        startTurn(*turnQueue.front());
    }



Player* Battle::getPlayerOfFighter(Fighter* fighter)
{
    if (fighter == nullptr) return nullptr;

    if (player1.getHero() != nullptr && player1.getHero()->getteam() == fighter->getteam())
        return &player1;
    if (player2.getHero() != nullptr && player2.getHero()->getteam() == fighter->getteam())
        return &player2;

    return nullptr;
}

void Battle::chooseHeroes(Player& first, Player& second)
{
    // انتخاب Hero دیگر از اینجا انجام نمی‌شود.
    // انتخاب Hero از طریق GUI انجام می‌شود.
}

void Battle::chooseHeroesGUI(
    Player& first,
    Player& second,
    int choice1,
    int choice2)
{
    vector<Fighter*> heroes = {
        &sherlock,
        &dracula,
        &invisibleman
    };

    // انتخاب Player 1
    first.chooseHero(heroes, choice1);

    // حذف قهرمان انتخاب شده
    heroes.erase(
        remove(heroes.begin(), heroes.end(), first.getHero()),
        heroes.end()
    );

    // انتخاب Player 2
    if (choice2 < 1 || choice2 > static_cast<int>(heroes.size()))
        throw std::runtime_error("Invalid Player 2 hero choice");

    second.chooseHero(heroes, choice2);

    // قهرمان انتخاب نشده را حذف می‌کنیم
    heroes.erase(
        remove(heroes.begin(), heroes.end(), second.getHero()),
        heroes.end()
    );

    // Hero انتخاب نشده می‌میرد
    for (Fighter* unchosen : heroes)
    {
        unchosen->setPosition(nullptr);
        unchosen->sethealth(0);
    }

    // راه‌اندازی موقعیت‌ها
    setuppositions();

    // ساخت Sidekick ها و واحدها
    first.getHero()->setupUnits(this, first);
    second.getHero()->setupUnits(this, second);
} 


void Battle::startTurn(Player& player)
{
    sherlockAbilityActive = false;
    Fighter* hero = player.getHero();

if (hero != nullptr && hero->isalive() && hero->getName() != "Dracula" && hero->getName() != "Sherlock")
    {
        hero->specialAbillity(this);
    }

    if (hero == &invisibleman)
    {
        checkInvisibleFogAtTurnStart();
    }
}

vector<Fighter*> Battle::getAllFighters() 
{
    vector<Fighter*> result;
    if (player1.getHero() && player1.getHero()->isalive()) result.push_back(player1.getHero());
    if (player2.getHero() && player2.getHero()->isalive()) result.push_back(player2.getHero());
    if (watson.isalive() && getPlayerOfFighter(&watson) != nullptr) result.push_back(&watson);
        
    for (Sisters& s : sisters) {
        if (s.isalive() && getPlayerOfFighter(&s) != nullptr) result.push_back(&s);
    }
    return result;
}

vector<Fighter*> Battle::getFighters()
{
    return getAllFighters();
}

void Battle::setuppositions() 
{
    Fighter* h1 = player1.getHero();
    Fighter* h2 = player2.getHero();
    if (h1 == nullptr || h2 == nullptr) return;

    if (rand() % 2 == 0)
     {
        h1->setPosition(map.getZone(1));
        h2->setPosition(map.getZone(2));
    } else
     {
        h1->setPosition(map.getZone(2));
        h2->setPosition(map.getZone(1));
    }
}

void Battle::chooseSidekickPosition(Player& player)
{
    cout << "\n-----------------------------------------------\n";
    if(player.getHero()->getName() == "Sherlock")
    {
        boardManager->showplacementzone(sherlock);
        int choice;
        do {
            choice = readInt(player.getName() + " Choose your sidekick's position.", 1, 32);
            if(getfighterat(map.getZone(choice))) cout << "occupied!\n";
        } while(getfighterat(map.getZone(choice)));
        watson.setPosition(map.getZone(choice));
    }
    else
    {
        boardManager->showplacementzone(dracula);
        for(int i = 0; i < 3; i++)
        {
            int choice;
            do {
                choice = readInt(player.getName() + " Choose your sidekick's position.", 1, 32);
                if(getfighterat(map.getZone(choice))) cout << "occupied!\n";
            } while(getfighterat(map.getZone(choice)));
            sisters[i].setPosition(map.getZone(choice));
        }
    }
}

void Battle::draculaability(Fighter* target)
{
    if(target == nullptr) return;
    target->takeDamage(1);
    cout << "Dracula damaged " << target->getName() << " for 1 damage\n";
    cout << "Dracula draws a card\n";
}

void Battle::chooseFogPosition(Player& player)
{
    Fighter* hero = player.getHero();
    vector<Zone*> zones = map.getplacementZone(hero->getPosition());
    vector<int> validids;

    cout << "\nChoose positions for Fog Tokens\n";
    for(int i = 0; i < 3; i++)
    {
        validids.clear();
        cout << "\nAvailable Zones:\n";
        for(Zone* z : zones)
        {
            bool used = false;
            for(FogToken& fog : fogtoken)
            {
                if(fog.getPosition() == z) { used = true; break; }
            }
            if(!used)
            {
                cout << z->getId() << " ";
                validids.push_back(z->getId());
            }
        }
        cout << endl;
        int choice = readchoice("Choose zone for Fog " + to_string(i + 1) + ": ", validids);
        fogtoken[i].setPosition(map.getZone(choice));
    }
}

vector<Sisters>& Battle::getsisters() { return sisters; }

Fighter* Battle::getfighterat(Zone* zone)
{
    if (zone == nullptr) return nullptr;
    if (dracula.isalive() && dracula.getPosition() == zone && getPlayerOfFighter(&dracula) != nullptr) return &dracula;
    if (invisibleman.isalive() && invisibleman.getPosition() == zone && getPlayerOfFighter(&invisibleman) != nullptr) return &invisibleman;
    if (sherlock.isalive() && sherlock.getPosition() == zone && getPlayerOfFighter(&sherlock) != nullptr) return &sherlock;
    if (watson.isalive() && watson.getPosition() == zone && getPlayerOfFighter(&watson) != nullptr) return &watson;

    for (size_t i = 0; i < sisters.size(); i++)
    {
        if (sisters[i].isalive() && sisters[i].getPosition() == zone && getPlayerOfFighter(&sisters[i]) != nullptr)
            return &sisters[i];
    }
    return nullptr; 
}

void Battle::printfighters()
{
    cout << "\n----------------BASKERVILLE------------------\n\n";
    cout << "RED TEAM\n";
    cout << "DRACULA HP : " << dracula.getHealth() << " / PLACE : " << dracula.getPosition()->getId() << endl;
    for(int i = 0; i < sisters.size(); i++)
    {
        cout << "SISTER (" << i + 1 << ") HP : " << sisters[i].getHealth() << " / PLACE : " << sisters[i].getPosition()->getId() << endl;
    }
    cout << "\nBLUE TEAM\n";
    cout << "SHERLOCK HP : " << sherlock.getHealth() << " / PLACE : " << sherlock.getPosition()->getId() << endl;
    cout << "WATSON HP : " << watson.getHealth() << " / PLACE : " << watson.getPosition()->getId() << endl;
}

void Battle::checkInvisibleFogAtTurnStart()
{
    invisibleStartedOnFog = false;
    Zone* pos = invisibleman.getPosition();
    for(FogToken& fog : fogtoken)
    {
        if(fog.getPosition() == pos) { invisibleStartedOnFog = true; break; }
    }
}


Player* Battle::getCurrentPlayerPtr() const
{
    if (turnQueue.empty())
        return nullptr;

    return turnQueue.front();
}