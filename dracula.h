#ifndef dracula_h
#define dracula_h

#include "fighter.h"

class Dracula : public Fighter
{
public:

    Dracula();

    void specialAbility(Fighter* target);
};

#endif