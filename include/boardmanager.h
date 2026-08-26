#ifndef BOARDMANAGER_H
#define BOARDMANAGER_H

#include "GameTypes.h"
#include <vector>

class Battle;
class Fighter;
class Zone;

class BoardManager {
private:
    Battle* battle; 
    bool canreach(Zone* current, Zone* target, int movesleft, Fighter& mover);

public:
    BoardManager(Battle* b) : battle(b) {}

    bool movefighter(Fighter& fighter, int destinationid, int Maxmove);
    bool areadjacent(Fighter& a, Fighter& b);
    void showPossiblemoves(Fighter& fight);
    void showplacementzone(Fighter& hero);
    ZoneCheckResult canEnterzone(Fighter* mover, Fighter* occupant, int moveleft);
    std::vector<Zone*> getReachableZone(Fighter& fighter, int maxMove);
    std::vector<Zone*> getReachableZoneFromZone(Zone* start, int maxMove);
    bool moveFogToken(int fogIndex, int destinationId);
    std::vector<int> getPlacementZoneIds(Fighter& hero);
};

#endif