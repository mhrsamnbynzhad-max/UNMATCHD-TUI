#include "battle.h"
#include "cardeffect.h"
#include <iostream>

using namespace std;

void Battle::combat(Fighter* attracker, Fighter* defender, int cardindex)
{
    ignoreAttackValue = false;
    ignoreDefendValue = false;

    finalAttackValue = 0;
    finalDefendValue = 0;

    if(attracker->handsize() == 0)
    {
        cout << "Atracker has no cards";
        return;
    }

    if(defender->handsize() == 0)
    {
        cout << "Defender has no cards";
        return;
    }

    Card attackcard = attracker->playcard(cardindex);
    this->lastAttackCard = attackcard;

    Card defendcard;

    bool isdefended = false;

    cout << defender->getName()
         << " Do you want to defend? (yes(1) or no(0))\n";

    int choose;
    cin >> choose;

    if(choose == 1 && defender->handsize() > 0)
    {
        isdefended = true;

        vector<int> defenseIndexes;

        for(int i = 0; i < defender->handsize(); i++)
        {
            Cardtype type =
                defender->gethand()[i].getcardType();

            if(type == DEFENSE || type == VERSATILE)
            {
                defenseIndexes.push_back(i);
            }
        }

        if(defenseIndexes.empty())
        {
            cout << defender->getName()
                 << " has no defense cards.\n";

            isdefended = false;
            defendcard = Card();
        }
        else
        {
            cout << "Choose a defense card:\n";

            for(int i = 0; i < defenseIndexes.size(); i++)
            {
                int idx = defenseIndexes[i];

                cout << i + 1 << ") "
                     << defender->gethand()[idx].getName()
                     << " (DEF "
                     << defender->gethand()[idx].getValue()
                     << ")\n";
            }

            int choice;
            cin >> choice;
            choice--;

            if(choice < 0 ||
               choice >= defenseIndexes.size())
            {
                cout << "Invalid choice.\n";

                isdefended = false;
                defendcard = Card();
            }
            else
            {
                int realIndex = defenseIndexes[choice];

                defendcard =
                    defender->playcard(realIndex);

                isdefended = true;
            }
        }
    }

    if(isdefended &&
       defendcard.getTiming() == BEFOR_COMBAT)
    {
        if(!this->getCancel())
        {
            applycardeffect(
                defendcard,
                defender,
                attracker
            );
        }
        else
        {
            cout << "Opponent card effect cancelled\n";
        }
    }

    finalAttackValue = attackcard.getValue();

    finalDefendValue =
        isdefended ? defendcard.getValue() : 0;

    if(ignoreAttackValue)
        finalAttackValue = 0;

    if(ignoreDefendValue)
        finalDefendValue = 0;

    if(attackcard.getTiming() == BEFOR_COMBAT)
    {
        applycardeffect(
            attackcard,
            attracker,
            defender
        );
    }

    if(isdefended &&
       defendcard.getTiming() == DURING_COMBAT)
    {
        applycardeffect(
            defendcard,
            defender,
            attracker
        );
    }

    if(attackcard.getTiming() == DURING_COMBAT)
    {
        applycardeffect(
            attackcard,
            attracker,
            defender
        );
    }

    lastFinaldefend = finalDefendValue;

    int damage =
        finalAttackValue - finalDefendValue;

    cout << "Attacker played : "
         << attackcard.getName()
         << endl;

    cout << "Defender played : "
         << defendcard.getName()
         << endl;

    if(damage > 0)
    {
        defender->takeDamage(damage);

        cout << "Damage (" << damage << ")\n";
    }

    if(isdefended &&
       defendcard.getTiming() == AFTER_COMBAT)
    {
        applycardeffect(
            defendcard,
            defender,
            attracker
        );
    }

    if(attackcard.getTiming() == AFTER_COMBAT)
    {
        applycardeffect(
            attackcard,
            attracker,
            defender
        );
    }

    this->setCancel(false);
}

void Battle::applycardeffect(
    Card& card,
    Fighter* attacker,
    Fighter* defender)
{
    CardEffect* effect = card.getEffect();

    if(effect)
    {
        effect->apply(
            attacker,
            defender,
            this,
            card
        );
    }
}