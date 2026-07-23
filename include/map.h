#ifndef MAP_H
#define MAP_H
#include <map>
#include "zone.h"
#include <vector>
using namespace std;

class Map
{
private:

    std::map<int , Zone*> zones;
    void connect(int  , int );
    std::vector<int >secretZones;

public:

    Map();
    Zone* getZone(int );
    const vector<int >&  getsecretZones()const;
    vector<Zone*> getplacementZone(Zone*);


    void printallzones();
    vector<Zone*> getZonebycolor(char);
    bool issecretzone(int);

};

#endif