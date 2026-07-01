#include "cardfactory.h"

using namespace std;


std::vector<Card> CardFactory::createDraculaDeck() {
    std::vector<Card> deck;

    for(const Cardinfo& info : Card::draculaCardDB)
    {
        for(int i = 0; i < info.count; i++)
        {
            deck.push_back(Card::createFromInfo(info));
        }
    }

    return deck;
}

std::vector<Card> CardFactory::createSherlockDeck() {
    std::vector<Card> deck;

    for(const Cardinfo& info : Card::sherlockCardDB)
    {
        for(int i = 0; i < info.count; i++)
        {
            deck.push_back(Card::createFromInfo(info));
        }
    }

    return deck;
}