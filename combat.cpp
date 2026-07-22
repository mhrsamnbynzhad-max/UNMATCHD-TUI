#include "battle.h"
#include "cardeffect.h"
#include <iostream>

using namespace std;

    void Battle :: combat(Fighter* attracker,Fighter* defender, int cardindex)
    {

        if(attracker->handsize() == 0)
        {
            cout<<"Atracker has no cards";
            return;
        }
         if(defender->handsize() == 0)
        {
            cout<<"Defender has no cards";
            return;
        }

        Card  attackcard = attracker->playcard(cardindex);
        this->lastAttackCard = attackcard;
        Card defendcard ;

        bool isdefended = false;
        cout<<defender->getName()<<"  Do yo want to defend? (yes(1) or no (0))\n";
        int choose;
        cin>>choose;

         if(choose == 1 && defender->handsize()>0)
         {
            isdefended = true;
            vector<int> defenseIndexes;

            for(int i = 0; i < defender->handsize(); i++)
            {
               Cardtype type = defender->gethand()[i].getcardType();

                if(type == DEFENSE || type == VERSATILE)
                {
                    defenseIndexes.push_back(i);
                }
            }

            if(defenseIndexes.empty())
            {
                cout << defender->getName() << " has no defense cards.\n";
                isdefended = false;
                defendcard = Card(); // NO_CARD
            }
            else
            {
                cout << "Choose a defense card:\n";
                for(int i = 0; i < defenseIndexes.size(); i++)
                {
                    int idx = defenseIndexes[i];

                    cout << i + 1 << ") "
                        << defender->gethand()[idx].getName()
                        << " (DEF " << defender->gethand()[idx].getValue() << ")\n";
                }

                int choice;
                cin >> choice;
                choice--;

                if(choice < 0 || choice >= defenseIndexes.size())
                {
                    cout << "Invalid choice.\n";
                    isdefended = false;
                    defendcard = Card();
                }
                else
                {
                    int realIndex = defenseIndexes[choice];
                    defendcard = defender->playcard(realIndex);
                    isdefended = true;
                }
                        
            }
            
         }
         else
         {
            isdefended = false;
            defendcard;
         }
        
         
         
         ExecuteOrder order = getexecuteCardeffect( attackcard,defendcard,attracker,defender,isdefended);
         
         applycardeffect(*order.acard, order.aowner, order.atarget );
         
         if(order.bcard != nullptr)
         {
            if(!getCancel())
            {
                applycardeffect( *order.bcard, order.bowner,order.btarget );
            }
            else
            {
                 cout<<"Opponent card effect was cancelled\n";
            }
         }
            
         int attackValue = attackcard.getValue();
         int defederValue = isdefended ? defendcard.getValue() : 0;
         lastFinaldefend = defederValue;
         int damage = attackValue - defederValue;
         
         cout<<"Attacker played :"<<attackcard.getName()<<endl;
         cout<<"Defender played :"<<defendcard.getName()<<endl;
         
         damage =  attackValue - defendcard.getValue();

         if(damage > 0 )
         {
             defender->takeDamage(damage);
             
             cout<<"Damage taken ( "<<damage<<" )" <<endl;
             cout<<"Attacker won the combat!" <<endl;
             
         }
         else
         {
             cout<<"Damage taken ( 0 )" <<endl;
             cout<<"Defender won the combat!" <<endl;   

         }
           
            this->setCancel(false);
    }

       void Battle :: applycardeffect(Card& card , Fighter* attacker ,Fighter* defender)
    {
           CardEffect* effect = card.getEffect();
           if(effect)
           {
              effect->apply(attacker,defender,this,card);
           }
    }

 
 