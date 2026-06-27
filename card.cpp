#include "Card.h"

Card::Card(Cardname cn,string n, string f, Cardtype c,CardTime ti , string e, int v,int b)
{
    cardname = cn;
    name = n;
    fighterType = f;
    cardType = c;
    effect = e;
    timing = ti;
    value = v;
    boost = b;
}

string Card::getName() const
{
    return name;
}

    Cardname  Card:: getcardname() const
    {
        return cardname;
    }


string Card::getfighterType() const
{
    return fighterType;
}

Cardtype Card::getcardType() const
{
    return cardType;
}

int Card::getValue() const
{
    return value;
}

int Card::getBoost() const
{
    return boost;
}