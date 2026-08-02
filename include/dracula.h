#ifndef dracula_h
#define dracula_h

#include "fighter.h"

class Dracula : public Fighter
{
public:

    Dracula();

    void specialabilityattack(Fighter* target);

    void specialAbillity(Battle* battle)override;

};

#endif