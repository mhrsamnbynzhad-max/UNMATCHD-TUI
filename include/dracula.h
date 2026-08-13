#ifndef dracula_h
#define dracula_h

#include "fighter.h"
#include "Player.h"

class Dracula : public Fighter
{
public:

    Dracula();

    void specialabilityattack(Fighter* target);

    void specialAbillity(Battle* battle)override;
    
    void setupUnits(Battle* , Player& ) override;

    std::vector<Fighter*> getAbilityTargets(Battle* );
    
    void useAbilityOn(Fighter* );
};

#endif