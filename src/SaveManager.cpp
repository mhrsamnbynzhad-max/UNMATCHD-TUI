#include "SaveManager.h"
#include "card.h"
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

static void saveFighter(ofstream& file, Fighter& f)
{
    // HP
    file << f.getHealth() << '\n';

    // Position
    if (f.getPosition() != nullptr)
        file << f.getPosition()->getId() << '\n';
    else
        file << -1 << '\n';

    // Hand
    const vector<Card>& hand = f.gethand();

    file << hand.size() << '\n';

    for (const Card& card : hand)
    {
        file << static_cast<int>(card.getcardname()) << ' '
             << card.getValue() << ' '
             << card.getBoost() << ' '
             << card.isValueLocked() << '\n';
    }

    // Deck
    const vector<Card>& deck = f.getdeck();

    file << deck.size() << '\n';

    for (const Card& card : deck)
    {
        file << static_cast<int>(card.getcardname()) << ' '
             << card.getValue() << ' '
             << card.getBoost() << ' '
             << card.isValueLocked() << '\n';
    }
}


static Card makeLoadedCard(
    int cardId,
    int value,
    int boost,
    bool locked,
    const vector<Cardinfo>& database)
{
    Cardname cardName = static_cast<Cardname>(cardId);

    for (const Cardinfo& info : database)
    {
        if (info.name == cardName)
        {
            Card card = Card::createFromInfo(info);

            card.forceValue(value);
            card.setBoost(boost);

            if (locked)
                card.lockValue();

            return card;
        }
    }

    return Card();
}


static void loadFighter(
    ifstream& file,
    Fighter& f,
    const vector<Cardinfo>& database,
    Battle& battle)
{
    int hp;
    int zoneId;

    // HP
    file >> hp;

    // Position
    file >> zoneId;

    f.sethealth(hp);

    if (zoneId == -1)
        f.setPosition(nullptr);
    else
        f.setPosition(battle.getMap().getZone(zoneId));

    // Hand
    int handSize;
    file >> handSize;

    f.gethand().clear();

    for (int i = 0; i < handSize; i++)
    {
        int cardId;
        int value;
        int boost;
        bool locked;

        file >> cardId >> value >> boost >> locked;

        Card card = makeLoadedCard(
            cardId,
            value,
            boost,
            locked,
            database
        );

        f.gethand().push_back(card);
    }

    // Deck
    int deckSize;
    file >> deckSize;

    vector<Card> deck;

    for (int i = 0; i < deckSize; i++)
    {
        int cardId;
        int value;
        int boost;
        bool locked;

        file >> cardId >> value >> boost >> locked;

        Card card = makeLoadedCard(
            cardId,
            value,
            boost,
            locked,
            database
        );

        deck.push_back(card);
    }

    f.setdeck(deck);
}


bool SaveManager::saveGame(Battle& battle, const string& filename)
{
    ofstream file(filename);

    if (!file.is_open())
        return false;

    // ---------------------------
    // GAME STATE
    // ---------------------------

    file << "GAME_STATE\n";

    // playerFirst
    file << battle.getplayerfirst() << '\n';

    // current player
    Player& current = battle.getCurrentPlayer();

    if (&current == &battle.getplayer1())
        file << 1 << '\n';
    else
        file << 2 << '\n';

    // actions this turn
    file << battle.getActionsThisTurn() << '\n';

    // abilities / flags
    file << battle.getsherlockability() << '\n';
    file << battle.getinvisibleactive() << '\n';
    file << battle.hasExtraAction() << '\n';
    file << battle.startedTurnOnFog() << '\n';
    file << battle.isgameover() << '\n';


    // ---------------------------
    // SELECTED HEROES
    // ---------------------------

    file << "PLAYER_HEROES\n";

    if (battle.getplayer1().getHero() != nullptr)
        file << battle.getplayer1().getHero()->getName() << '\n';
    else
        file << "NONE\n";

    if (battle.getplayer2().getHero() != nullptr)
        file << battle.getplayer2().getHero()->getName() << '\n';
    else
        file << "NONE\n";


    // ---------------------------
    // FIGHTERS
    // ---------------------------

    file << "FIGHTERS\n";

    file << "SHERLOCK\n";
    saveFighter(file, battle.getSherlock());

    file << "WATSON\n";
    saveFighter(file, battle.getWatson());

    file << "DRACULA\n";
    saveFighter(file, battle.getDracual());

    file << "INVISIBLE\n";
    saveFighter(file, battle.getInvisibleMan());


    // ---------------------------
    // SISTERS
    // ---------------------------

    file << "SISTERS\n";

    vector<Sisters>& sisters = battle.getsisters();

    file << sisters.size() << '\n';

    for (Sisters& sister : sisters)
        saveFighter(file, sister);





    // ---------------------------
    // FOG TOKENS
    // ---------------------------

    file << "FOG\n";

    vector<FogToken>& fogs = battle.getfogtoken();

    file << fogs.size() << '\n';

    for (FogToken& fog : fogs)
    {
        if (fog.getPosition() != nullptr)
            file << fog.getPosition()->getId() << '\n';
        else
            file << -1 << '\n';
    }


    file.close();

    return true;
}


bool SaveManager::loadGame(Battle& battle, const string& filename)
{
    ifstream file(filename);

    if (!file.is_open())
        return false;

    string section;

    // ---------------------------
    // GAME STATE
    // ---------------------------

    file >> section;

    if (section != "GAME_STATE")
        return false;

    bool playerFirst;
    int currentPlayer;
    int actions;

    bool sherlockAbility;
    bool invisibleAbility;
    bool extraAction;
    bool invisibleFog;
    bool gameOver;

    file >> playerFirst;
    file >> currentPlayer;
    file >> actions;

    file >> sherlockAbility;
    file >> invisibleAbility;
    file >> extraAction;
    file >> invisibleFog;
    file >> gameOver;


    // ---------------------------
    // PLAYER HEROES
    // ---------------------------

    file >> section;

    if (section != "PLAYER_HEROES")
        return false;

    string hero1;
    string hero2;

    file >> hero1;
    file >> hero2;


    if (hero1 == "Sherlock")
        battle.getplayer1().setHero(&battle.getSherlock());

    else if (hero1 == "Dracula")
        battle.getplayer1().setHero(&battle.getDracual());

    else if (hero1 == "InvisibleMan")
        battle.getplayer1().setHero(&battle.getInvisibleMan());

    else
        battle.getplayer1().setHero(nullptr);


    if (hero2 == "Sherlock")
        battle.getplayer2().setHero(&battle.getSherlock());

    else if (hero2 == "Dracula")
        battle.getplayer2().setHero(&battle.getDracual());

    else if (hero2 == "InvisibleMan")
        battle.getplayer2().setHero(&battle.getInvisibleMan());

    else
        battle.getplayer2().setHero(nullptr);


    // ---------------------------
    // FIGHTERS
    // ---------------------------

    file >> section;

    if (section != "FIGHTERS")
        return false;

    file >> section;

    if (section == "SHERLOCK")
    {
        loadFighter(
            file,
            battle.getSherlock(),
            Card::sherlockCardDB,
            battle
        );
    }

    file >> section;

    if (section == "WATSON")
    {
        loadFighter(
            file,
            battle.getWatson(),
            Card::sherlockCardDB,
            battle
        );
    }

    file >> section;

    if (section == "DRACULA")
    {
        loadFighter(
            file,
            battle.getDracual(),
            Card::draculaCardDB,
            battle
        );
    }

    file >> section;

    if (section == "INVISIBLE")
    {
        loadFighter(
            file,
            battle.getInvisibleMan(),
            Card::invisiblemanCardDB,
            battle
        );
    }


    // ---------------------------
    // SISTERS
    // ---------------------------

    file >> section;

    if (section != "SISTERS")
        return false;

    int sisterCount;
    file >> sisterCount;

    vector<Sisters>& sisters = battle.getsisters();

    for (int i = 0; i < sisterCount && i < (int)sisters.size(); i++)
    {
        loadFighter(
            file,
            sisters[i],
            Card::draculaCardDB,
            battle
        );
    }




        // ---------------------------
        // FOG TOKENS
        // ---------------------------

        file >> section;

        if (section != "FOG")
            return false;

        int fogCount;
        file >> fogCount;

        vector<FogToken>& fogs = battle.getfogtoken();

        for (int i = 0; i < fogCount && i < (int)fogs.size(); i++)
        {
            int zoneId;
            file >> zoneId;

            if (zoneId == -1)
                fogs[i].setPosition(nullptr);
            else
                fogs[i].setPosition(
                    battle.getMap().getZone(zoneId)
                );
        }


    // ---------------------------
    // RESTORE GAME STATE
    // ---------------------------

    battle.setPlayerFirst(playerFirst);

    battle.setActionsThisTurn(actions);

    battle.setGameState(
        gameOver,
        sherlockAbility,
        invisibleAbility,
        extraAction,
        invisibleFog
    );


    if (currentPlayer == 1)
        battle.setCurrentPlayer(&battle.getplayer1());
    else
        battle.setCurrentPlayer(&battle.getplayer2());


    file.close();

    return true;
}