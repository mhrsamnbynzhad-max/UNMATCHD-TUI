#ifndef CARDFACTORY_H
#define CARDFACTORY_H

#include "card.h"
#include "cardeffect.h"
#include <vector>

class Fighter;
class battle;

class CardFactory {
public:
    static std::vector<Card> createDraculaDeck();
    static std::vector<Card> createSherlockDeck();
};


#endif