#include "Zone.h"


Zone::Zone(int id, char color)
{
    this->id = id;

    colors.push_back(color);
}


Zone::Zone(int id , std::vector<char> colors)
{
    this->id = id;

    this->colors = colors;
}


int Zone::getId() const
{
    return id;
}


const std::vector<char>& Zone::getColors() const
{
    return colors;
}


bool Zone::hasColor(char color) const
{
    for(char c : colors)
    {
        if(c == color)
            return true;
    }

    return false;
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
    secret = true;
    secretTarget = zone;
}


bool Zone::issecret() const
{
    return secret;
}


Zone* Zone::getsecrettarget() const
{
    return secretTarget;
}