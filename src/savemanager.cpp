#include "savemanager.h"
#include "battle.h"
#include "fighter.h"
#include "card.h"
#include "cardfactory.h"

#include <fstream>
#include <iostream>
#include <sstream>


static bool findCardById(
    const std::vector<Card>& deck,
    int id,
    Card& result)
{
    for (const Card& card : deck)
    {
        if (static_cast<int>(card.getcardname()) == id)
        {
            result = card;
            return true;
        }
    }

    return false;
}

static bool loadFighter(
    std::ifstream& file,
    Fighter& fighter)
{
    std::string line;

    int health = 0;
    int positionId = -1;

    std::vector<int> deckIds;
    std::vector<int> handIds;

    while (std::getline(file, line))
    {
        if (line == "END_FIGHTER")
            break;

        std::stringstream ss(line);
        std::string key;

        ss >> key;

        if (key == "HEALTH")
        {
            ss >> health;
        }
        else if (key == "POSITION")
        {
            ss >> positionId;
        }
        else if (key == "DECK_CARD")
        {
            int id;
            ss >> id;
            deckIds.push_back(id);
        }
        else if (key == "HAND_CARD")
        {
            int id;
            ss >> id;
            handIds.push_back(id);
        }
    }

    // تعیین Deck اصلی Fighter
    std::vector<Card> originalDeck;

    if (fighter.getteam() == SHERLOCK)
    {
        originalDeck = CardFactory::createSherlockDeck();
    }
    else if (fighter.getteam() == DRACULA)
    {
        originalDeck = CardFactory::createDraculaDeck();
    }
    else if (fighter.getteam() == INVISIBLE)
    {
        originalDeck = CardFactory::createInvisibleDeck();
    }

    std::vector<Card> savedDeck;
    std::vector<Card> savedHand;

    // بازسازی Deck
    for (int id : deckIds)
    {
        Card card;

        if (findCardById(originalDeck, id, card))
        {
            savedDeck.push_back(card);
        }
    }

    // بازسازی Hand
    for (int id : handIds)
    {
        Card card;

        if (findCardById(originalDeck, id, card))
        {
            savedHand.push_back(card);
        }
    }

    fighter.sethealth(health);
    fighter.setdeck(savedDeck);
    fighter.sethand(savedHand);

    return true;
}

static void saveFighter(std::ofstream& file, const Fighter& fighter)
{
    file << "FIGHTER\n";

    file << "NAME " << fighter.getName() << "\n";
    file << "HEALTH " << fighter.getHealth() << "\n";

    if (fighter.getPosition() != nullptr)
        file << "POSITION " << fighter.getPosition()->getId() << "\n";
    else
        file << "POSITION -1\n";

    // Deck
    file << "DECK_SIZE " << fighter.getDeck().size() << "\n";

    for (const Card& card : fighter.getDeck())
    {
        file << "DECK_CARD "
             << static_cast<int>(card.getcardname()) << "\n";
    }

    // Hand
    file << "HAND_SIZE " << fighter.getHand().size() << "\n";

    for (const Card& card : fighter.getHand())
    {
        file << "HAND_CARD "
             << static_cast<int>(card.getcardname()) << "\n";
    }

    file << "END_FIGHTER\n";
}






bool SaveManager::saveGame(const Battle& battle, const std::string& filename)
{
    std::ofstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Could not open save file!\n";
        return false;
    }

    file << "UNMATCHED_SAVE\n";

    file << "GAMEOVER "
         << battle.isgameover() << "\n";

    file << "SHERLOCK_ABILITY "
         << battle.getSherlockAbility() << "\n";

    file << "INVISIBLE_ABILITY "
         << battle.getInvisibleAbility() << "\n";

    file << "EXTRA_ACTION "
         << battle.getExtraAction() << "\n";

    file << "ACTIONS "
         << battle.getActionsThisTurn() << "\n";

    file << "SIDEKICK_INDEX "
         << battle.getSidekickIndex() << "\n";

    file << "FOG_INDEX "
         << battle.getFogIndex() << "\n";

    saveFighter(file, battle.getSherlock());
    saveFighter(file, battle.getWatson());
    saveFighter(file, battle.getDracual());
    saveFighter(file, battle.getInvisibleMan());

    file.close();

    std::cout << "Game saved successfully!\n";

    return true;
}








static bool loadFighterPosition(
    std::ifstream& file,
    Battle& battle,
    Fighter& fighter)
{
    std::string line;

    while (std::getline(file, line))
    {
        if (line.rfind("POSITION ", 0) == 0)
        {
            int zoneId = std::stoi(line.substr(9));

            if (zoneId != -1)
            {
                Zone* zone = battle.getMap().getZone(zoneId);

                if (zone == nullptr)
                {
                    std::cerr << "Invalid zone ID: "
                              << zoneId << "\n";
                    return false;
                }

                fighter.setPosition(zone);
            }

            return true;
        }

        if (line == "END_FIGHTER")
            break;
    }

    return false;
}



bool SaveManager::loadGame(Battle& battle, const std::string& filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Could not open save file!\n";
        return false;
    }

    std::string line;

    // Header
    std::getline(file, line);

    if (line != "UNMATCHED_SAVE")
    {
        std::cerr << "Invalid save file!\n";
        return false;
    }

    while (std::getline(file, line))
    {
        if (line == "FIGHTER")
        {
            std::getline(file, line);

            if (line == "NAME Sherlock")
            {
                loadFighterPosition(file, battle, battle.getSherlock());
            }
            else if (line == "NAME Watson")
            {
                loadFighterPosition(file, battle, battle.getWatson());
            }
            else if (line == "NAME Dracula")
            {
                loadFighterPosition(file, battle, battle.getDracual());
            }
            else if (line == "NAME Invisible Man")
            {
                loadFighterPosition(
                    file,
                    battle,
                    battle.getInvisibleMan()
                );
            }
        }
    }

    file.close();

    std::cout << "Game loaded successfully!\n";

    return true;
}