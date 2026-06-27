#ifndef BATTLE_H
#define BATTLE_H

#include "fighter.h"
#include "dracula.h"
#include "watson.h"
#include "sherlock.h"
#include "map.h"
#include <vector>

class Battle
{
private:

    Sherlock sherlock;
    Watson watson;
    Dracula dracula;

    std::vector<Fighter> sisters;

    Map map;
    bool canreach(Zone*  , Zone*  , int  , Fighter&);

public:

    Battle();

    void setuppositions();

    void draculaability(Fighter* target);

    void printfighters();

    void builddraculadeck();
    void combat(Fighter*,Fighter*  );

    void buildsherlockdeck();
    void applycardeffect(Card);


    bool areadjacent(Fighter& ,Fighter&  );

    bool movefighter(Fighter& ,int  , int );
    void showPossiblemoves(Fighter&);
    Fighter* getfighterat(Zone* );
    void showplacementzone(Fighter& );
   
    
};

#endif