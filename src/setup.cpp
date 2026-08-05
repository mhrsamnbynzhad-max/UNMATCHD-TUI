#include "battle.h"
#include "handling.h"
#include <iostream>

using namespace std;

   void Battle:: setuppositions()
    {
        sherlock.setPosition(map.getZone(1));
        dracula.setPosition(map.getZone(2));
        cout<<dracula.getPosition()->getId()<<endl;
         
    }

        void Battle::chooseSidekickPosition(Player& player)
        {
            cout<<"\n-----------------------------------------------\n";

            if(player.getHero()->getName() == "Sherlock")
            {
                showplacementzone(sherlock);

                int choice;

                do
                {
                     choice = readInt(player.getName() + " Choose your sidekick's position." , 1 , 32);

                    if(getfighterat(map.getZone(choice)))
                    cout << "occupied!\n";

                } while(getfighterat(map.getZone(choice)));

                watson.setPosition(map.getZone(choice));
            }
            else
            {
                showplacementzone(dracula);

                for(int i = 0; i < 3; i++)
                {
                    int choice;

                    do
                    {

                       choice = readInt(player.getName() + " Choose your sidekick's position." , 1 , 32);
                        if(getfighterat(map.getZone(choice)))
                            cout << "occupied!\n";

                    } while(getfighterat(map.getZone(choice)));

                    sisters[i].setPosition(map.getZone(choice));
                }
            }
        }
   
            void  Battle :: showplacementzone(Fighter& hero )
            {
                vector<Zone*>zones = map.getplacementZone(hero.getPosition());
                cout<<"\nAvailable zones";


                for(int i = 0 ; i <zones.size() ; i ++)
                {
                    if(getfighterat(zones[i] )== nullptr)
                    {
                        cout<<zones[i]->getId()<<" ";    

                    }
                }
                cout<<endl;
            }
    

        void Battle::draculaability(Fighter* target)
        {
            if(target == nullptr)
            {
                return;
            }
            
            target->takeDamage(1);
            
            cout << "Dracula damaged "
                << target->getName()
                << " for 1 damage\n";

            cout << "Dracula draws a card\n";
        }