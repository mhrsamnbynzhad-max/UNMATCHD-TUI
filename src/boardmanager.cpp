#include "BoardManager.h"
#include "battle.h"
#include <iostream>
#include <queue>
#include <set>

using namespace std;

bool BoardManager::canreach(Zone* current, Zone* target, int movesleft, Fighter& mover)
{
    if(current == target) return true;
    if(movesleft <= 0) return false;

    for(Zone* next : current->getNei())
    {
        Fighter* occupant = battle->getfighterat(next);
        if(occupant != nullptr)
        {
            if(occupant->getteam() != mover.getteam()) continue;
        }

        if(canreach(next, target, movesleft - 1, mover)) return true;
    }

    if(mover.getteam() == INVISIBLE)
    {
        bool onFog = false;
        for(FogToken& fog : battle->getfogtoken())
        {
            if(fog.getPosition() == current) { onFog = true; break; }
        }

        if(onFog)
        {
            for(FogToken& fog : battle->getfogtoken())
            {
                Zone* next = fog.getPosition();
                if(next == current) continue;

                Fighter* occupant = battle->getfighterat(next);
                if(occupant != nullptr && occupant->getteam() != mover.getteam()) continue;

                if(canreach(next, target, movesleft - 1, mover)) return true;
            }
        }
    }

    if(battle->getMap().issecretzone(current->getId()))
    {
        const vector<int>& secret = battle->getMap().getsecretZones();
        for(int id : secret)
        {
            if(id == current->getId()) continue;
            Zone* next = battle->getMap().getZone(id);
            Fighter* occ = battle->getfighterat(next);

            if(occ && occ->getteam() != mover.getteam()) continue; 

            if(canreach(next, target, movesleft - 1, mover)) return true;
        }
    }

    return false;
}

vector<Zone*> BoardManager::getReachableZone(Fighter& fighter, int maxMove)
{
    vector<Zone*> result;
    result.push_back(fighter.getPosition());

    for(int i = 1; i <= 32; i++)
    {
        Zone* z = battle->getMap().getZone(i);
        if(z == fighter.getPosition()) continue;

        if(canreach(fighter.getPosition(), z, maxMove, fighter))
        {
            if(battle->getfighterat(z) == nullptr) result.push_back(z);
        }
    }
    return result;
}

bool BoardManager::movefighter(Fighter& fighter, int destinationid, int Maxmove)
{
    Zone* destination = battle->getMap().getZone(destinationid);
    if(destination == fighter.getPosition()) return true;
    if(destination == nullptr) return false;  

    if(!canreach(fighter.getPosition(), destination, Maxmove, fighter)) return false;
    
    Fighter* occupant = battle->getfighterat(destination);
    if(occupant != nullptr)
    {
        cout << "Can't stop there\n";
        return false;
    }
    
    fighter.setPosition(destination);
    return true;
}

bool BoardManager::areadjacent(Fighter& a, Fighter& b)
{
    for(auto n : a.getPosition()->getNei())
    {
        if(n == b.getPosition()) return true;
    }
    if(battle->getMap().issecretzone(a.getPosition()->getId()) && 
       battle->getMap().issecretzone(b.getPosition()->getId()) && 
       a.getPosition() != b.getPosition())
    {
        return true; 
    }
    return false;
}

void BoardManager::showPossiblemoves(Fighter& fight)
{
    Zone* now = fight.getPosition();
    cout << "Possible Moves : \n";
    for(Zone* n : now->getNei())
    {
        cout << n->getId() << " ";
    }
    cout << endl;
}

vector<int> BoardManager::getPlacementZoneIds(Fighter& hero) {
    vector<Zone*> zones = battle->getMap().getplacementZone(hero.getPosition());
    vector<int> ids;
    for(Zone* z : zones) {
        if(battle->getfighterat(z) == nullptr) {
            ids.push_back(z->getId());
        }
    }
    return ids;
}
  void BoardManager::showplacementzone(Fighter& hero)
{
    vector<int> ids = getPlacementZoneIds(hero);
    cout << "\nAvailable zones: ";
    for(int id : ids)
    {
        cout << id << " ";
    }
    cout << endl;
}


ZoneCheckResult BoardManager::canEnterzone(Fighter* mover, Fighter* occupant, int moveleft)
{
    ZoneCheckResult result;
    result.allow = true;
    if(!occupant) return result;

    string name = occupant->getName();
    if(name == "Sherlock" || name == "Watson")
    {
        result.allow = false;
        result.blocker = name;
        return result;
    }

    if(name == "Sister")
    {
        if(moveleft == 0)
        {
            result.allow = false;
            result.blocker = "Sister";
            return result;
        }
        result.allow = true;
        result.blocker = "Sister";
        return result;
    }
    return result;
}

bool BoardManager::moveFogToken(int fogIndex, int destinationId)
{
    vector<FogToken>& fogtokens = battle->getfogtoken();
    if(fogIndex < 0 || fogIndex >= fogtokens.size()) return false;

    Zone* destination = battle->getMap().getZone(destinationId);
    if(destination == nullptr) return false;

    for(size_t i = 0; i < fogtokens.size(); i++)
    {
        if((int)i != fogIndex && fogtokens[i].getPosition() == destination)
        {
            cout << "There is already a Fog Token here\n";
            return false;
        }
    }

    fogtokens[fogIndex].setPosition(destination);
    return true;
}

vector<Zone*> BoardManager::getReachableZoneFromZone(Zone* start, int maxMove)
{
    vector<Zone*> result;
    queue<pair<Zone*, int>> q;
    set<Zone*> visited;

    q.push({start, 0});
    visited.insert(start);

    vector<FogToken>& fogtokens = battle->getfogtoken();

    while(!q.empty())
    {
        Zone* current = q.front().first;
        int dist = q.front().second;
        q.pop();

        if(dist == maxMove) continue;

        for(Zone* next : current->getNei())
        {
            if(visited.count(next)) continue;
            visited.insert(next);

            bool hasFog = false;
            for(FogToken& fog : fogtokens)
            {
                if(fog.getPosition() == next && next != start) { hasFog = true; break; }
            }

            if(!hasFog) result.push_back(next);
            q.push({next, dist + 1});
        }
        
        if(battle->getMap().issecretzone(current->getId()))
        {
            for(int id : battle->getMap().getsecretZones())
            {
                if(id == current->getId()) continue;
                Zone* next = battle->getMap().getZone(id);
                if(visited.count(next)) continue;
                visited.insert(next);

                bool hasFog = false;
                for(FogToken& fog : fogtokens)
                {
                    if(fog.getPosition() == next && next != start) { hasFog = true; break; }
                }

                if(!hasFog) result.push_back(next);
                q.push({next, dist + 1});
            }
        }
    }
    return result;
}