#include"map.h"
#include<iostream>
using namespace std;
Map::Map()
{
    zones[1]  = new Zone(1,'P');//perpule Zone
    zones[2]  = new Zone(2,'R');
    zones[3]  = new Zone(3,'P');
    zones[4]  = new Zone(4,'G');//Green Zone
    zones[5]  = new Zone(5,'G');
    zones[6]  = new Zone(6,'G');
    zones[7]  = new Zone(7,'G');

    zones[8]  = new Zone(8,'D'); // Dark blue Zone
    zones[9]  = new Zone(9,'D');

    zones[10] = new Zone(10,'B');
    zones[11] = new Zone(11,'L');
    zones[12] = new Zone(12,'B');
    zones[13] = new Zone(13,'L');
    zones[14] = new Zone(14,'B');
    zones[15] = new Zone(15,'B');

    zones[16] = new Zone(16,'B');

    zones[17] = new Zone(17,'P');

    zones[18] = new Zone(18,'R');// Brown Zone
    zones[19] = new Zone(19,'R');
    zones[20] = new Zone(20,'R');

    zones[21] = new Zone(21,'G');

    zones[22] = new Zone(22,'Y');//Yellow Zone
    zones[23] = new Zone(23,'Y');
    zones[24] = new Zone(24,'Y');
    zones[25] = new Zone(25,'P');

    zones[26] = new Zone(26,'X');//Gray Zone
    zones[27] = new Zone(27,'X');
    zones[28] = new Zone(28,'X');
    zones[29] = new Zone(29,'X');
    zones[30] = new Zone(30,'X');
    zones[31] = new Zone(31,'X');
    zones[32] = new Zone(32,'X');
    connect(1,17);

    connect(3,17);

    connect(17,25);

    connect(25,26);
    connect(25,27);

    connect(26,27);

    connect(27,28);

    connect(28,4);
    connect(28,29);

    connect(29,30);
    connect(29,7);

    connect(30,31);
    connect(30,7);

    connect(31,32);

    connect(32,9);

    connect(9,10);

    connect(10,11);

    connect(11,12);
    connect(11,13);

    connect(13,14);

    connect(14,15);

    connect(15,16);

    connect(16,2);

    connect(2,12);
    connect(2,18);

    connect(18,19);
    connect(18,20);

    connect(19,20);

    connect(19,21);
    connect(20,21);

    connect(21,22);
    connect(21,24);
    connect(21,4);

    connect(22,23);
    connect(22,24);

    connect(24,3);

    connect(4,1);

    connect(4,5);
    connect(5,6);

    connect(6,7);
    connect(6,8);

    connect(7,28);

    connect(8,9);

    connect(4,27);
    secretZones.push_back(15);
    secretZones.push_back(23);
    secretZones.push_back(32);
}
void Map::connect(int a , int b)
{
    zones[a]->addneighbor(zones[b]);
    zones[b]->addneighbor(zones[a]);
}

void Map::printallzones()
{
    for(auto& pair : zones)
    {
        Zone* z = pair.second;

         cout<< "Zone "<< z->getId()<< "----> ";

        for(Zone* n : z->getNei())
        {
             cout <<"["<< n->getId()<< "]";
        }

        cout<<'\n';

        if(z->issecret())
        {
             cout<< endl << "**** Secret:"<< z->getsecrettarget()->getId();
        }

         cout << '\n';
    }
    
}

    const vector<int >& Map:: getsecretZones()const
    {
               return secretZones;
    }


    bool  Map :: issecretzone(int id)
    {
         for(int i = 0 ; i<secretZones.size() ; i ++)
         {
            if(secretZones[i] == id)
            {
                return  true;
            }
         }
         return false;
    }

    Zone*  Map :: getZone(int id )
    {
           return zones[id];
    }
    vector<Zone*> Map :: getZonebycolor(char color)
    {
        vector<Zone*>result;
        for(auto& pair : zones)
        {
            if(pair.second->getColor() == color )
            {
                result.push_back(pair.second);
            }
        }
       return result;
    }

    vector<Zone*> Map :: getplacementZone(Zone* heroplace)
    {
        vector<Zone*> result;
        char color = heroplace->getColor();

        for(auto& pair : zones )
        {
            if(pair.second->getColor() == color)
            {

              result.push_back(pair.second);
            }
        }

        return result;
    }
 