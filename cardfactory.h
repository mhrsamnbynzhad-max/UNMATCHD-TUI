#ifndef CARDFACTORY_H
#define CARDFACTORY_H

#include "Card.h"
#include <vector>

class CardFactory {
public:
    static std::vector<Card> createDraculaDeck();
    static std::vector<Card> createSherlockDeck();
};

#endif