#ifndef FOGTOKEN_h
#define FOGTOKEN_h
#include "zone.h"

class FogToken
{
private:
    Zone* position;
public:
    FogToken();
    void setPosition(Zone*);
    Zone* getPosition() const;
};

#endif