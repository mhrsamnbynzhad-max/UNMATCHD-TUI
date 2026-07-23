#ifndef ZONE_H
#define ZONE_H

#include <vector>

class Zone
{
private:

    int id;
    std:: vector<char> colors;

    bool secret = false;
    Zone* secretTarget = nullptr;

    std::vector<Zone*> neighbor;

public:

    Zone(int  , char );
    Zone(int ,std::vector<char>);
    

    int getId() const;
    const std::vector<char>& getColors() const;

    bool hasColor(char) const;

    void addneighbor(Zone* ); //Add Neighbors...

    const std::vector<Zone*>& getNei() const;//get Neighbors

    void setsecret(Zone* );

    bool issecret() const ;

    Zone* getsecrettarget() const ;//Get Secret Target...

    std::vector<Zone*> getplacementZone(Zone*);
   
};

#endif