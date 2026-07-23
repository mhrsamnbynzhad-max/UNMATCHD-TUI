#include "Zone.h"

Zone::Zone(int id, char color)
{
    this->id = id;
    colors.push_back(color);
}

int Zone::getId() const
{
    return id;
}

char Zone::getColor() const
{
    return colors[0];
}

void Zone::addneighbor(Zone* zone)
{
    neighbor.push_back(zone);
}

const std::vector<Zone*>& Zone::getNei() const
{
    return neighbor;
}

void Zone::setsecret(Zone* zone)
{
    secret =  true;
    secretTarget= zone;
}

bool Zone::issecret() const
{
    return secret;
}

Zone* Zone::getsecrettarget() const
{
    return secretTarget;
}