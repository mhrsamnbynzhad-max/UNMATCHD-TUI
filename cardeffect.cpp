#include "card.h"
#include "fighter.h"
#include "battle.h"
#include "zone.h"
#include "cardeffect.h"

using namespace std;

void BloodThirstEffect :: apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card)
  {
                Zone* enemyZone = defender->getPosition();          // Place of defender
                char color = enemyZone->getColor();                 // Color of defender place

                vector<Zone*> sameColorZones = battle->getMap().getZonebycolor(color);   // All places that is the same color 

                int counter = 0;

                for (const Fighter& sis : battle->getsisters())                 
                {
                    Zone* sisPos = sis.getPosition();               // Sister's place

                    for (Zone* zone : sameColorZones)               // Check the places that share the same color as the defender's place 
                    {
                        if (sisPos->getId() == zone->getId())       // If a sister is found on any of the same-colored places, increase the counter
                        {
                            counter++;
                            if (counter == 3) break;                // The maximum allowed value for the counter is 3 .
                        }
                    }
                    if (counter == 3) break;
                }

               card.setValue(card.getValue()+ counter);                       // Increases attack 
 }
       
