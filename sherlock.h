#ifndef SHERLOCK_H
#define SHERLOCK_H

#include "fighter.h"

class Sherlock : public Fighter
{
public:

    Sherlock();

    void specialAbility(Fighter* target);
};

#endif