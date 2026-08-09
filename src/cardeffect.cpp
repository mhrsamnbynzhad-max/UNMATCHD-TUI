#include "card.h"
#include "cardeffect.h"
#include "fighter.h"
#include "battle.h"
#include "combatmanager.h"
#include "boardmanager.h"
#include "handling.h"
#include "zone.h"

#include  <iostream>
#include  <algorithm>
using namespace std;

void BloodThirstEffect :: apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card)
  {
       Zone* enemyZone = defender->getPosition();

    int counter = 0;

   for(Fighter* fighter : battle->getFighters())
{
    if(fighter->getName() != "Sister")
        continue;

    if(!fighter->isalive())
        continue;

    Zone* sisZone = fighter->getPosition();

    bool sameColor = false;

    for(char enemyColor : enemyZone->getColors())
    {
        if(sisZone->hasColor(enemyColor))
        {
            sameColor = true;
            break;
        }
    }

    if(sameColor)
        counter++;
}

    card.setValue(card.getValue() + counter);

    cout << "Blood Thirst: +" << counter
         << " attack.\n";
 }
       
void AmbushEffect :: apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card)
{
        if(!defender || defender->handsize() == 0)
        {
            return;
        }

        int  index = rand() % defender->handsize();  
        Card remove = defender->remove_ranodmcard();

        int opponentBoost = remove.getBoost();
        card.setValue(card.getValue() +  opponentBoost);

        cout<<"Ambush : opponent discarded "<<remove.getName()<<" and + " <<opponentBoost <<" attack added . \n";

}

 void FeastEffect :: apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card)
{
        attacker->heal(2);
        cout<<"Feast: Dracula healed +2 HP. \n";

           vector<Sisters>& sisters = battle->getsisters();
           vector<int> deadindexes;

        for(int i = 0 ; i< (int)sisters.size(); i++)
        {
            if(!sisters[i].isalive())
            {
                deadindexes.push_back(i);
            }
        }

        if(deadindexes.empty())
        {
            cout<<"Feast : No ddead sister to revive.\n";
            return;
        }
        int idx = deadindexes[0];
        Fighter& revived = sisters[idx];

        revived.heal(1);

       Zone* draculaZone = attacker->getPosition();
       revived.setPosition(draculaZone);

       cout<<"Feast : "<<revived.getName()<<" revived with 1 hp at dracula's zone.\n ";

}

void MistFormEffect::apply(Fighter* attacker,  Fighter* defender, Battle* battle, Card& card)
{
    cout << "\nMist Form activated!\n";

    Fighter& dracula = battle->getDracual();

    vector<Zone*> zones = battle->getMap().getplacementZone(dracula.getPosition());

    vector<int>validids;

    for(int i = 0; i < zones.size(); i++)
    {
    
        if(battle->getfighterat(zones[i]) == nullptr )
        {
            cout << zones[i]->getId() << " ";

            validids.push_back(zones[i]->getId());
        
        }
        else
        {
            cout<<dracula.getPosition()->getId()<<"\n";
            validids.push_back(dracula.getPosition()->getId());
            break;
        }
    }

    cout << endl;

    int choice;
    choice = readchoice("Choose a zone for Dracula:\n " , validids);


    Zone* newZone = battle->getMap().getZone(choice);
    if(isNull(newZone , "Invalid zone"))
    return;


    if(battle->getfighterat(newZone) == nullptr)
    {
        dracula.setPosition(newZone);

        cout << "Dracula moved to zone " << newZone->getId() << endl;
        battle->giveExtraAction();
    }
    else
    {
        cout << "Invalid zone!\n";
        return;
    }
}
 void MonesterFormEffect :: apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card) 
 {
    cout<<"hand size beformonesform "<<attacker->gethand().size()<<endl;
    int basattack = card.getValue();
    int bouns = 0;
    vector<Card>& hand = attacker->gethand();
    cout<<" you may discard  cards too increase attack\n";

    while (true)
    {
        if(hand.empty())
        {
            cout<<"your hand is  empty no mmore discard possible\n";
            break;
        }
        cout<<"\nyour hand\n";
        for(int  i = 0 ; i <hand.size(); i ++)
        {
            cout<< i+1 <<"]"<<hand[i].getName()<<"\n";
        }
            
            int choose;
            choose = readInt("enter your card index to discard(or 0 to stop)" , 0 , hand.size());
            choose --;
            if(choose == -1)
            {
                cout<<"you stopped dicarding \n";
                break;
            }
            if(choose<-1 || choose >= hand.size())
            {
                cout<<"invalid...\n";
                continue;
            }
            cout<<"discard : "<<hand[choose].getName()<<"\n";
            hand.erase(hand.begin()+choose);
            bouns++;
            cout<<"attack increases by +1 (bonus = )"<<bouns<<"\n";
        }
            int lastattack = basattack + bouns;
              cout<<"final attack : " <<lastattack<<"\n";
              card.setValue(lastattack);  
 }

  void ManeuverEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card)
{
    Fighter* dracula = nullptr;
    Fighter* enemy = nullptr;

    if (attacker != nullptr && attacker->getName() == "Dracula")
    {
        dracula = attacker;
        enemy = defender;
    }
    else if (defender != nullptr && defender->getName() == "Dracula")
    {
        dracula = defender;
        enemy = attacker;
    }
    else
    {
        cout << "Only Dracula can move 3 places.\n";
        return;
    }

    Zone* current = dracula->getPosition();
    int moves = 0;

    while (moves < 3)
    {
        cout << "\n----- Dracula Movement (" << moves << "/3) -----\n";
        cout << "Current Zone: " << current->getId() << "\n";

        vector<Zone*> connectedzone = current->getNei();
        vector<Zone*> validmoves;

        cout << "Available connected zones:\n";

        for (Zone* z : connectedzone)
        {
            Fighter* occupant = battle->getfighterat(z);

            if (occupant == nullptr)
            {
                validmoves.push_back(z);
                cout << "Zone " << z->getId() << " (OK - Empty)\n";
            }
            else if (occupant->getteam() == dracula->getteam())
            {
                validmoves.push_back(z);
                cout << "Zone " << z->getId() << " Passable (" << occupant->getName() << " - Teammate)\n";
            }
            else
            {
                cout << "Zone " << z->getId() << " Blocked by Enemy (" << occupant->getName() << ")\n";
            }
        }

        if (validmoves.empty())
        {
            cout << "No valid moves available. Movement ends.\n";
            break;
        }

        vector<int> validids;
        for (Zone* z : validmoves)
        {
            validids.push_back(z->getId());
        }

        int choice = readchoice("Enter zone ID to move Dracula:\n", validids);
        Zone* selected = nullptr;

        for (Zone* z : validmoves)
        {
            if (z->getId() == choice)
            {
                selected = z;
                break;
            }
        }

        if (selected == nullptr)
        {
            cout << "Invalid zone. Try again.\n";
            continue;
        }

        dracula->setPosition(selected);
        current = selected;
        moves++;
        cout << "Dracula moved to zone " << current->getId() << "\n";

        if (moves >= 3)
        {
            Fighter* occupant = battle->getfighterat(current);
            if (occupant != nullptr && occupant != dracula && occupant->getteam() == dracula->getteam())
            {
                cout << "Warning: Cannot end movement on a teammate's zone!\n";
            }
            cout << "Maximum movement reached (3).\n";
            break;
        }

        Fighter* occupant = battle->getfighterat(current);
        if (occupant != nullptr && occupant != dracula && occupant->getteam() == dracula->getteam())
        {
            cout << "You are on a teammate's zone (" << occupant->getName() << "). You MUST keep moving!\n";
            continue;
        }
        int ok = readInt("Do you want to continue moving? Yes (1) / No (0):\n", 0, 1);
        if (ok == 0)
        {
            cout << "Movement stopped by player.\n";
            break;
        }
    }
}

  void  ExploitEffect :: apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card) 
  {
    auto cards = attacker->getrandomcard(1);
    if(!cards.empty())
    {
        attacker->addtohand(cards);
        cout<<"Draw a card . added to your hand .\n";
    }
  }

  void LookIntoMyEyesEffect :: apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card)
  {
    int oppBoost = battle->getCombat()->gelastattackcard().getBoost();

    card.setValue(card.getValue() + oppBoost);

    cout<<" opponent  boost ("<<oppBoost<<" 0 added to this defense.\n";
  }

void HuntEyesEffect :: apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card) 
{
  Zone* attackerzone = attacker->getPosition();
  vector<Zone*> connected = attackerzone->getNei();

  int  healcount = 0;

  for (Zone* place : connected)
  {
    Fighter* novalid = battle->getfighterat(place);

    if(novalid != nullptr)
    {
        if(novalid->getteam() != attacker->getteam())
        {
            cout<<"Hunt : opponent found in zone "<< place->getId()<<"\n";

            novalid->takeDamage(1);
            cout<<novalid->getName()<<" take 1 damage .\n";

            healcount++;
        }
    }
  }
  
 for (int i = 0; i < healcount; i++)
 {
    cout<<"dracula hp "<<attacker->getHealth()<<"/"<<attacker->getMaxealth()<<endl;
    if(attacker->getHealth() < attacker->getMaxealth())
    {
        attacker->heal(1);
        cout<<"Dracula haled 1 HP (current HP : "<<attacker->getHealth()<<")\n";
    }

 }
 
}

void SeductivecallEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card)
{
     vector<Fighter*> fighters;
    
    for(Fighter* f : battle->getFighters())
    {
        if(f != nullptr && f->isalive() && f->getPosition() != nullptr)
        {
            fighters.push_back(f);
        }
    }
    
    if (fighters.empty())
    {
        cout << "No fighters available on the board to move.\n";
        return;
    }
    cout << "\n--- Seductive Call: Choose a fighter to move (up to 2 spaces) ---\n";
    for (int i = 0; i < fighters.size(); i++)
    {
        cout << i + 1 << ") " << fighters[i]->getName() << " (Zone " << fighters[i]->getPosition()->getId() << ")\n";
    }

    int choose = readInt("Choose fighter number: ", 1, fighters.size());
    choose--;
   
    Fighter* selected = fighters[choose];

    vector<Zone*> reachableZones = battle->getBoard()->getReachableZone(*selected, 2);
    vector<Zone*> validZones;

    cout << "Available destination zones (distance <= 2 & empty):\n";
    for(Zone* z : reachableZones)
    {
        Fighter* occ = battle->getfighterat(z);
        if(occ == nullptr || occ == selected)
        {
            cout << z->getId() << "  ";
            validZones.push_back(z);
        }
    }
    cout << "\n";

    if(validZones.empty())
    {
        cout << "No valid destination zones available for movement.\n";
        return;
    }

    vector<int> validids;
    for(Zone* z : validZones)
    {
        validids.push_back(z->getId());
    }

    int destId = readchoice("Enter destination zone ID: \n", validids);
    
    Zone* targetZone = nullptr;
    for(Zone* z : validZones)
    {
        if(z->getId() == destId)
        {
            targetZone = z;
            break;
        }
    }

    if(targetZone == nullptr)
    {
        cout << "Invalid destination.\n";
        return;
    }

    selected->setPosition(targetZone);
    cout << fighters[choose]->getName() << " moved to Zone " << targetZone->getId() << "\n";

    if(selected->getName() == "Sister")
    {
        cout << "The selected fighter is a Sister. No damage dealt.\n";
        return;
    }

    vector<Zone*> connectedToTarget = targetZone->getNei();
    int sisterCount = 0;

    for(Zone* z : connectedToTarget)
    {
        Fighter* occ = battle->getfighterat(z);
        if(occ != nullptr && occ->getName() == "Sister")
        {
            sisterCount++;
        }
    }

    if(sisterCount > 0)
    {
        int totalDamage = sisterCount * 1; // هر خواهر 1 دمیج
        cout << "Found " << sisterCount << " Sister(s) nearby! " << selected->getName() << " takes " << totalDamage << " damage.\n";
        selected->takeDamage(totalDamage);
    }
    else
    {
        cout << "No Sister found in adjacent zones. No damage taken.\n";
    }
}

 void  SurvivalInstinctEffect :: apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card)
 {
     // 1) Calculate combat result
    int finalAttack = card.getValue();
    int finalDefense = battle->getCombat()->getlastdefend();

    cout << "Final Attack = " << finalAttack << " | Final Defense = " << finalDefense << endl;

    if(finalAttack <= finalDefense)
    {
        cout << "The Sister did not win the combat. Nothing happens.\n";
        return;
    }

    cout << "The Sister won the combat! Dracula may be placed next to the enemy fighter.\n";

    // 2) Choose opponent target (Sherlock or Watson)
        vector<Fighter*> opponent;
        for(Fighter* f : battle->getFighters())
        {
            if(f->getteam() != attacker->getteam())
            {
                opponent.push_back(f);
            }
        }

    for(int i = 0; i < opponent.size(); i++)
    {
        cout << i+1 << ") " << opponent[i]->getName()<< " (Zone " << opponent[i]->getPosition()->getId() << ")\n";
    }

    int choice;
    choice = readInt( "Dracula wants to move next to which opponent?\n", 1 , opponent.size());
    choice--;

    Fighter* targetopponent= opponent[choice];
    Zone* opponentZone = targetopponent->getPosition();

    // 3) Find adjacent zones to the opponent
    vector<Zone*> neighbors = opponentZone->getNei();
    vector<Zone*> emptyZones;

    cout << "Adjacent zones to the enemy:\n";
    for(Zone* z : neighbors)
    {
        Fighter* occ = battle->getfighterat(z);
        cout << "Zone " << z->getId();

        if(occ == nullptr)
        {
            cout << " (Empty)\n";
            emptyZones.push_back(z);
        }
        else
        {
            cout << " (Occupied by " << occ->getName() << ")\n";
        }
    }


    if(emptyZones.empty())
    {
        cout << "There are no empty adjacent zones. Dracula cannot be moved.\n";
        return;
    }

    // 4) Choose empty zone for Dracula
    for(int i = 0; i < emptyZones.size(); i++)
    {
        cout << i+1 << ") Zone " << emptyZones[i]->getId() << endl;
    }

    int destChoice;
    destChoice = readInt("Dracula can move to these empty zones:\n" , 0 , emptyZones.size());
    destChoice--;
    
    Zone* finalZone = emptyZones[destChoice];

    // 5) Move Dracula
    Fighter& dracula = *attacker;
    dracula.setPosition(finalZone);

    cout << "Dracula moved to Zone " << finalZone->getId() << ".\n";
 }

 void FeintEffect :: apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card)
 {
    cout<<"Opponent card's effect is cancelled\n";
    battle->getCombat()->setCancel(true);
 }



// ---------------- Counter Attack ----------------

void CounterAttackEffect::apply(Fighter* attacker,    Fighter* defender,   Battle* battle,   Card& card)
{
    if(!attacker || !defender)
        return;


    if(battle->getBoard()->areadjacent(*attacker,*defender))
    {
        defender->takeDamage(2);

        cout<<"Counter Attack: "
            <<defender->getName()
            <<" takes 2 damage.\n";
    }
    else
    {
        cout<<"Counter Attack failed: fighters are not adjacent.\n";
    }
}



// ---------------- Fixed Point ----------------
void FixedPointEffect::apply(Fighter* attacker,  Fighter* defender,  Battle* battle,  Card& card)
{
    if(attacker == nullptr)
        return;

    Fighter* sidekick = nullptr;

    for(Fighter* f : battle->getFighters())
    {
        if(f->getName()=="Watson")
        {
            sidekick = f;
            break;
        }
    }

    if(sidekick == nullptr)
        return;

    if(battle->getBoard()->areadjacent(*attacker,*sidekick))
    {
        attacker->heal(1);
        sidekick->heal(1);
        cout<<"Fixed Point activated!\n";
    }
    else
    {
        cout<<"Watson is not adjacent.\n";
    }
}



// ---------------- Service ----------------

void ServiceEffect::apply(Fighter* attacker,Fighter* defender,Battle* battle,Card& card)
{

    Fighter* sherlock = attacker;
    Fighter* watson = nullptr;
    for(Fighter* f : battle->getFighters())
    {
        if(f->getName()=="Watson")
        {
            watson=f;
            break;
        }
    }

    if(watson == nullptr)
        return;
        
    Zone* sherlockzone  = sherlock->getPosition();

    vector<Zone*> ok;
    for(Zone*z : sherlockzone->getNei())
    {
        if(battle->getfighterat(z) == nullptr)
        ok.push_back(z);
    }
    if(!ok.empty())
    {
        cout<<"Choose a zone adjacent to Sherlock\n";
    
         battle->getSherlock().heal(2);

    for(int i = 0; i < ok.size(); i++)
            cout << i + 1 << ") Zone " << ok[i]->getId() << endl;

        int choice;
        choice = readInt("Choose a zone adjacent to Sherlock\n" , 1 , ok.size());
        choice--;
        
        if(choice >= 0 && choice < ok.size())
        {
            watson->setPosition(ok[choice]);
            
         cout << "Watson moved to Zone "<< ok[choice]->getId() << endl;
        }
    }
    else
    {
        cout << "No empty adjacent zone for Watson.\n";
    }
    
    sherlock->heal(1);
    cout<<"Service: Sherlock healed 1 HP.\n";

      vector<Card> cards = attacker->getrandomcard(1);

        if(!cards.empty())
        {
            attacker->addtohand(cards);
            cout << "Drew 1 card.\n";
        }

    }




// ---------------- Study Method ----------------

void StudyMethodEffect::apply(Fighter* attacker,    Fighter* defender,    Battle* battle,    Card& card)
{
    if(attacker == nullptr || defender == nullptr)
        return;


    if(attacker->getName() != "Sherlock")
    {
        return;
    }


    int damage = battle->getCombat()->getFinalAttackValue() - battle->getCombat()->getFinalDefendValue();


    if(damage <= 0)
    {
        cout << "Sherlock did not win the battle.\n";
        return;
    }


    cout << "\nSherlock looks at enemy hand:\n";


    vector<Card>& enemyHand = defender->gethand();


    if(enemyHand.empty())
    {
        cout << "Enemy hand is empty.\n";
        return;
    }


    for(int i = 0; i < enemyHand.size(); i++)
    {
        cout << i + 1 << ") "
             << enemyHand[i].getName()
             << endl;
    }
}



// ---------------- Elementary ----------------

void ElementaryEffect::apply(Fighter* attacker,    Fighter* defender,    Battle* battle,    Card& card)
{
    int guess;
    cout<<"Elementary Prediction\n";
    guess = readInt("Guess attack value: " , 1 , 6 );
   
    int realAttack = battle->getCombat()->getFinalAttackValue();

    if(guess == realAttack)
    {
        card.setValue(card.getValue()+2);

        cout<<"Correct prediction +2 defense\n";
    }
    else
    {
        cout<<"Wrong prediction\n";
    }

}



// ---------------- Impossible ----------------
void ImpossibleEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card)
{
    if(attacker == nullptr || defender == nullptr)
    return;

    int guess;
    guess = readInt("Predict opponent attack value: " , 1 , 6);


    Card opponentAttack = battle->getCombat()->gelastattackcard();


    int realAttack = opponentAttack.getValue();


    if(guess == realAttack)
    {
        cout << "Correct prediction!\n";

        battle->getCombat()->setCancel(1);

        battle->getCombat()->setIgnoreAttack(true);
    }
    else
    {
        cout << "Wrong prediction!\n";
    }
}



// ---------------- Master Of Disguise ----------------

void MasterOfDisguiseEffect::apply(Fighter* attacker, Fighter* defender,  Battle* battle,  Card& card)
{
    Fighter* sherlock = nullptr;
    Fighter* dracula = nullptr;

    for(Fighter* f : battle->getFighters())
    {
        if(f->getName()=="Sherlock")
            sherlock = f;

        if(f->getName()=="Dracula")
            dracula = f;
    }

    if(sherlock == nullptr || dracula == nullptr)
    {
        cout<<"Master Of Disguise failed.\n";
        return;
    }

    Zone* sherlockZone = sherlock->getPosition();
    Zone* draculaZone = dracula->getPosition();

    sherlock->setPosition(draculaZone);
    dracula->setPosition(sherlockZone);

    dracula->takeDamage(1);

    cout<<"Master of Disguise activated!\n";
    cout<<"Sherlock and Dracula swapped positions.\n";
    cout<<"Dracula takes 1 damage.\n";
       
}



// ---------------- Game On ----------------

void GameOnEffect::apply(Fighter* attacker,  Fighter* defender,   Battle* battle,   Card& card)
{
    if(attacker == nullptr || battle == nullptr)
        return;


    cout << "Game On Effect: Move 3 zones\n";


    vector<Zone*> reachable =  battle->getBoard()->getReachableZone(*attacker, 3);


    if(reachable.empty())
    {
        cout << "No available zone!\n";
        return;
    }


    cout << "Available zones:\n";

    for(int i = 0; i < reachable.size(); i++)
    {
        cout << i + 1 
             << ") Zone "
             << reachable[i]->getId()
             << endl;
    }


    int choice;
    cin >> choice;


    if(choice < 1 || choice > reachable.size())
    {
        cout << "Invalid choice\n";
        return;
    }


    Zone* targetZone = reachable[choice - 1];


    int distance = 0;

    for(auto z : reachable)
    {
        if(z == targetZone)
            break;

        distance++;
    }


    battle->getBoard()->movefighter( *attacker, targetZone->getId(), 3 );


    cout << attacker->getName() << " moved to zone " << targetZone->getId() << endl;
}


// ---------------- Confirm Suspicion ----------------

void ConfirmSuspicionEffect::apply(Fighter* attacker,
                                   Fighter* defender,
                                   Battle* battle,
                                   Card& card)
{

    if(!defender)
        return;


    cout<<"Confirm Suspicion activated.\n";


    if(defender->handsize()>0)
    {
        cout<<"Enemy has "
            <<defender->handsize()
            <<" cards.\n";
    }

}



// ---------------- Strategic Deduction ----------------

void StrategicDeductionEffect::apply( Fighter* attacker, Fighter* defender, Battle* battle, Card& card)
{
    if(defender == nullptr)
        return;

    vector<Card>& hand = defender->gethand();


    if(hand.empty())
        return;

    for(int i = 0; i < hand.size(); i++)
    {
        cout << i+1 << ") "<< hand[i].getName()<< " Value: "<< hand[i].getValue()<< " Boost: "<< hand[i].getBoost()<< endl;
    }


    int choice;
    choice = readInt("Choose opponent card to change boost:\n" ,1 , hand.size());

    if(choice < 1 || choice > hand.size())
        return;


    Card& enemyCard = hand[choice-1];


    int printedValue = enemyCard.getValue();

    enemyCard.setBoost(printedValue);


    cout << "Strategic Deduction activated!\n";
    cout << enemyCard.getName()<< " boost changed to "<< printedValue<< endl;
}



// ---------------- Learning Never Ends ----------------

void LearningNeverEndsEffect::apply( Fighter* attacker, Fighter* defender, Battle* battle, Card& card)
{
    if(attacker == nullptr || defender == nullptr)
        return;


    int damage = battle->getCombat()->getFinalAttackValue()-battle->getCombat()->getFinalDefendValue();


    if(damage > 0)
    {
        cout << "Learning Never Ends: Opponent draws 1 card\n";
        defender->drawBoostMovement();
    }


  
    else
    {
        cout << "Learning Never Ends: You draw 2 cards\n";
        attacker->drawBoostMovement();
        attacker->drawBoostMovement();
    }
}



// ---------------- Deception ----------------

void DeceptionEffect::apply(Fighter* attacker,Fighter* defender,Battle* battle,Card& card)
{
    if(attacker == nullptr || defender == nullptr)
        return;

    vector<Card>& enemyHand = defender->gethand();

    if(enemyHand.empty())
    {
        cout << "Opponent has no cards to discard.\n";
        return;
    }


    cout << "\nOpponent hand:\n";

    for(int i = 0; i < enemyHand.size(); i++)
    {
        cout << i + 1 << ") "
             << enemyHand[i].getName()
             << endl;
    }


    int choice;
    choice = readInt( "Choose a card to burn: " , 1 , enemyHand.size());

    cout << enemyHand[choice-1].getName() << " was burned.\n";

    enemyHand.erase(enemyHand.begin() + (choice - 1));
}



// ---------------- Sidearm ----------------

void SidearmEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card)
{

    if(!defender)
        return;

    defender->takeDamage(card.getValue() );
    cout<<"Sidearm deals "<<card.getValue()<<" damage.\n";
}

void movetoken ( Battle* battle , int v)
{
     vector<FogToken>& fogs = battle->getfogtoken();

    cout<<"\nChoose Fog token:\n";

    for(int i=0;i<fogs.size();i++)
    {
        cout<<i+1<<") Fog "<<i+1  <<" Zone " <<fogs[i].getPosition()->getId() <<endl;
    }
    int fogchoice = readInt("Fog : ",1,fogs.size());
    FogToken& selectedFog = fogs[fogchoice-1];

    // 3) move fog 0-3 spaces

    vector<Zone*> reachable = battle->getBoard()->getReachableZoneFromZone(selectedFog.getPosition(), v);
    
     vector<Zone*> temp;

        for(Zone* z : reachable)
        {
            if(battle->getfighterat(z)==nullptr)
                temp.push_back(z);
        }

        reachable = temp;


        vector<int> ids;
        ids.push_back(0);

        cout<<"0) Don't move Fog\n";

        for(Zone* z : reachable)
        {
            cout<<z->getId()<<" ";ids.push_back(z->getId());
        }
        cout<<endl;


        int destination = readchoice("Move Fog: ", ids);

        if(destination != 0)
        {
            selectedFog.setPosition( battle->getMap().getZone(destination) );
        }

}

void CodedNotesEffect::apply(Fighter* attacker,  Fighter* defender,  Battle* battle,  Card& card)
{
    if(attacker == nullptr)
        return;

    cout << "\n===== CODED NOTES =====\n";

    // ---------- Draw 3 ----------
    for(int i = 0; i < 3; i++)
    {
        Card c = attacker->drawTopCard();

        if(c.getName() == "")
            break;

        attacker->gethand().push_back(c);

        cout << "Draw : "<< c.getName()<< endl;
    }

    vector<Card>& hand = attacker->gethand();

    if(hand.size() < 2)
        return;

    cout << "\nCurrent Hand\n";

    for(int i = 0; i < hand.size(); i++)
    {
        cout << i + 1<< ") "<< hand[i].getName()<< endl;
    }

    int first =
    readInt("Choose first card : ", 1, hand.size());

    Card firstCard = hand[first-1];

    hand.erase(hand.begin() + (first-1));

    cout << "\nRemaining Hand\n";

    for(int i = 0; i < hand.size(); i++)
    {
        cout << i + 1 << ") " << hand[i].getName() << endl;
    }

    int second = readInt("Choose second card : ", 1, hand.size());

    Card secondCard = hand[second-1];

    hand.erase(hand.begin() + (second-1));

    cout << "\nWhich card should be on TOP of deck?\n";
    cout << "1) "<< firstCard.getName()<< endl;

    cout << "2) " << secondCard.getName() << endl;

    int order = readInt("Choice : ",1,2);

    if(order == 1)
    {
        attacker->putCardOnTop(secondCard);
        attacker->putCardOnTop(firstCard);
    }
    else
    {
        attacker->putCardOnTop(firstCard);
        attacker->putCardOnTop(secondCard);
    }

    cout << "\nCards returned to top of deck.\n";
}

void ConfoundEffect::apply(Fighter* attacker, Fighter* defender,   Battle* battle,   Card& card)
{
    if(attacker == nullptr || defender == nullptr)
        return;

    cout << "\n===== CONFOUND =====\n";

    // ---------- Opponent may discard ----------
    cout << defender->getName()
         << ", discard one card?\n";

    cout << "0) No\n";

    vector<Card>& hand = defender->gethand();

    for(int i = 0; i < hand.size(); i++)
    {
        cout << i + 1  << ") "  << hand[i].getName()  << endl;
    }

    int choice =
    readInt("Choice : ", 0, hand.size());

    if(choice != 0)
    {
        cout << hand[choice-1].getName() << " discarded.\n";

        hand.erase(hand.begin() + (choice-1));

        return;
    }

    // ---------- Move Fog ----------
    vector<FogToken>& fogs = battle->getfogtoken();

    vector<bool> moved(fogs.size(), false);

    while(true)
    {
        cout << "\nChoose a Fog Token to move\n";
        cout << "0) Finish\n";

        bool anyLeft = false;

        for(int i = 0; i < fogs.size(); i++)
        {
            if(moved[i])
                continue;

            anyLeft = true;

            cout << i + 1 << ") Fog "<< i + 1<< " (Zone " << fogs[i].getPosition()->getId() << ")\n";
        }

        if(!anyLeft)
            break;

        int fogChoice =readInt("Choice : ",   0,fogs.size());

        if(fogChoice == 0)
            break;

        fogChoice--;

        if(moved[fogChoice])
        {
            cout << "This Fog Token has already been moved.\n";
            continue;
        }

        FogToken& fog = fogs[fogChoice];

        vector<int> validZones;

        for(int id = 1; id <= 32; id++)
        {
            Zone* z = battle->getMap().getZone(id);

            bool occupiedByFog = false;

            for(int j = 0; j < fogs.size(); j++)
            {
                if(j == fogChoice)
                    continue;

                if(fogs[j].getPosition() == z)
                {
                    occupiedByFog = true;
                    break;
                }
            }

            if(!occupiedByFog)
                validZones.push_back(id);
        }

        int zoneId = readchoice("Destination Zone : ", validZones);

        fog.setPosition( battle->getMap().getZone(zoneId) );

        moved[fogChoice] = true;

        cout << "Fog moved to Zone "<< zoneId<< endl;
    }
    }

 void CovertPreparationEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card)
{
    if(attacker == nullptr)
        return;

    // 1) Draw one card
    Card c = attacker->drawTopCard();
    if(c.getName() != "")
    {
        attacker->gethand().push_back(c);
        cout<<"Drew card : "<<c.getName()<<endl;
    }

    // 2) choose one fog
    vector<FogToken>& fogs = battle->getfogtoken();

    cout<<"\nChoose Fog token:\n";

    for(int i=0;i<fogs.size();i++)
    {
        cout<<i+1<<") Fog "<<i+1 <<" Zone " <<fogs[i].getPosition()->getId() <<endl;
    }
    int choice = readInt("Fog : ",1,fogs.size());
    FogToken& selectedFog = fogs[choice-1];

    // 3) move fog 0-2 spaces
    cout<<"0) Don't move\n";

    vector<Zone*> reachable = battle->getBoard()->getReachableZoneFromZone(selectedFog.getPosition(), 2);

    for(int i=0;i<reachable.size();i++)
    {
        cout<<i+1<<") Zone "
            <<reachable[i]->getId()
            <<endl;
    }
    int move =readInt("Move fog : ",  0, reachable.size());

    if(move != 0)
    {
        selectedFog.setPosition(
            reachable[move-1]
        );
    }

    Fighter* enemy = defender;
    vector<FogToken*> otherFogs;

    for(int i=0;i<fogs.size();i++)
    {
        if(i != choice-1)
            otherFogs.push_back(&fogs[i]);
    }

    bool close = false;


    for(FogToken* fog : otherFogs)
    {
        vector<Zone*> zones =battle->getBoard()->getReachableZoneFromZone(    fog->getPosition(),    2);
        for(Zone* z : zones)
        {
            if(z == enemy->getPosition())
            {
                close = true;
                break;
            }
        }


        if(close)
            break;
    }

    // already close -> nothing

    if(close)
    {
        cout<<"Enemy is already near a fog.\n";
        return;
    }

    // 5) move enemy near closest fog
  cout<<"Enemy is moved near Fog.\n";


    FogToken* targetFog = otherFogs[0];

    vector<Zone*> possible;
    vector<Zone*> around = battle->getMap().getplacementZone(targetFog->getPosition());


    for(Zone* z : around)
    {
        vector<Zone*> dist =battle->getBoard()->getReachableZoneFromZone(targetFog->getPosition(),2);
        for(Zone* x : dist)
        {
            if(x == z &&
               battle->getfighterat(z)==nullptr)
            {
                possible.push_back(z);
            }
        }
    }



    if(!possible.empty())
    {
        enemy->setPosition(possible[0]);
        cout<<enemy->getName() <<" moved near Fog Zone " <<targetFog->getPosition()->getId() <<endl;
    }

}

void DreamingOfRevengeEffect::apply(Fighter* attacker,  Fighter* defender,  Battle* battle,  Card& card)
{
    if(attacker == nullptr || battle == nullptr)
        return;

    cout<<"\n===== DREAMING OF REVENGE =====\n";

    bool onFog = false;

    vector<FogToken>& fogs =battle->getfogtoken();

    for(FogToken& fog : fogs)
    {
        if(fog.getPosition() == attacker->getPosition())
        {
            onFog = true;
            break;
        }
    }

    if(!onFog)
    {
        cout<<"Invisible Man is not on Fog.\n";
        return;
    }

        vector<Fighter*> enemies = battle->getFighters();

        for(auto it = enemies.begin(); it != enemies.end(); )
        {
            if((*it)->getteam() == attacker->getteam())
                it = enemies.erase(it);
            else
                ++it;
        }

    // 3) Damage enemies on Fog

    for(Fighter* enemy : enemies)
    {
        if(enemy == nullptr || !enemy->isalive())
            continue;


        for(FogToken& fog : fogs)
        {
            if(enemy->getPosition() == fog.getPosition())
            {
                enemy->takeDamage(1);
                cout<<enemy->getName()<<" takes 1 damage.\n";
                break;
            }
        }
    }

}

void EmergefrommistEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card)
{
    if(attacker == nullptr || battle == nullptr)
        return;

    if(battle->startedTurnOnFog())
    {
        card.setValue(5);
    }
    
}

 void ImpossibletoseeEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card)
{
  
    Card* attackCard = battle->getCombat()->getCurrentAttackCard();
    Card* defendCard = battle->getCombat()->getCurrentDefendCard();
    Card* enemyCard = nullptr;

    if(attackCard == nullptr || defendCard == nullptr)
        return;

    if (attackCard == &card) 
    {
        enemyCard = defendCard; 
    }
    else if (defendCard == &card) 
    {
        enemyCard = attackCard; 
    }
    else 
    {
        if (attackCard->getName() == card.getName()) {
            enemyCard = defendCard;
        } else {
            enemyCard = attackCard;
        }
    }

    if(enemyCard != nullptr)
    {
        enemyCard->forceValue(0);
        enemyCard->lockValue();
        cout << "Invisible Man Effect! Enemy card (" << enemyCard->getName() << ") value becomes 0.\n";
    }
}


void IntoThinAirEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card)
{
    if(attacker == nullptr)
        return;

    cout << "\n--- Disappear In Air ---\n";

    vector<Zone*> neighbors = attacker->getPosition()->getNei();
    vector<int> validMove;
    validMove.push_back(0);

    cout << "Nearby zones:\n";
    cout << "0) Stay here\n";

    for(Zone* z : neighbors)
    {
        if(battle->getfighterat(z)==nullptr)
        {
            cout << z->getId() << endl;
            validMove.push_back(z->getId());
        }
    }
    int choice = readchoice( "Move InvisibleMan: ", validMove );

    if(choice != 0)
    {
        attacker->setPosition( battle->getMap().getZone(choice) );
        cout<<"InvisibleMan moved to "<<choice<<endl;
    }
    
    movetoken(battle , 3);

}

void LurkingEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card)
{
    if(attacker == nullptr)
        return;

    cout << "\n--- Lurking ---\n";
    attacker->addtohand(attacker->getrandomcard(1));

    cout << "1) Move Invisible Man to a Fog Token\n";
    cout << "2) Move a Fog Token up to 3 spaces\n";

    int choice = readInt("Choose: ",1,2);

    vector<FogToken>& fogs = battle->getfogtoken();

    if(choice == 1)
    {
        vector<int> ids;


        cout<<"Fog token positions:\n";

        for(int i=0;i<fogs.size();i++)
        {
            cout<<i+1<<") Fog "<<i+1  <<" Zone "<<fogs[i].getPosition()->getId() <<endl;
            ids.push_back(i);
        }
        int selected = readchoice("Choose fog: ",ids);

        attacker->setPosition( fogs[selected].getPosition());
    }

    else if(choice == 2)
    {
       movetoken(battle , 3);
    }
}

void ReignOfTerrorEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card)
{
    if(attacker == nullptr)
        return;

    Zone* currentZone = attacker->getPosition();

    bool hasFog = false;

    for(FogToken& fog : battle->getfogtoken())
    {
        if(fog.getPosition() == currentZone)
        {
            hasFog = true;
            break;
        }
    }

    if(!hasFog)
    {
        cout << "No Fog Token here. Effect failed.\n";
        return;
    }
    cout << "Invisible Man attacks through the fog!\n";

     vector<Fighter*> enemies = battle->getFighters();


        for(auto it = enemies.begin(); it != enemies.end(); )
        {
            if((*it)->getteam() == attacker->getteam())
                it = enemies.erase(it);
            else
                ++it;
        }

    // 3) Damage enemies on Fog

    for(Fighter* enemy : enemies)
    {
        if(enemy != nullptr && enemy->isalive())
        {
            enemy->takeDamage(2);
            cout<<enemy->getName()<<" takes 2 damage.\n";
        }
    }
}

void RollingFogEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card)
{
     vector<FogToken>& fogs = battle->getfogtoken();

    vector<bool> moved(fogs.size(), false);

    while(true)
    {
        cout << "\nChoose a Fog Token to move\n";
        cout << "0) Finish\n";

        bool anyLeft = false;

        for(int i = 0; i < fogs.size(); i++)
        {
            if(moved[i])
                continue;

            anyLeft = true;

            cout << i + 1 << ") Fog "<< i + 1<< " (Zone " << fogs[i].getPosition()->getId() << ")\n";
            
        }

        if(!anyLeft)
            break;

        int fogChoice =readInt("Choice : ",   0,fogs.size());

        if(fogChoice == 0)
            break;

        fogChoice--;

        if(moved[fogChoice])
        {
            cout << "This Fog Token has already been moved.\n";
            continue;
        }

        FogToken& fog = fogs[fogChoice];

        vector<int> validZones;

        for(int id = 1; id <= 32; id++)
        {
            Zone* z = battle->getMap().getZone(id);

            bool occupiedByFog = false;

            for(int j = 0; j < fogs.size(); j++)
            {
                if(j == fogChoice)
                    continue;

                if(fogs[j].getPosition() == z)
                {
                    occupiedByFog = true;
                    break;
                }
            }

            if(!occupiedByFog)
                validZones.push_back(id);
        }

        int zoneId = readchoice("Destination Zone : ", validZones);

        fog.setPosition( battle->getMap().getZone(zoneId) );

        moved[fogChoice] = true;

        cout << "Fog moved to Zone "<< zoneId<< endl;
    }
    battle->giveExtraAction();
    cout << "Extra action gained!\n";
}


void SlipAwayEffect::apply(Fighter* attacker,
                           Fighter* defender,
                           Battle* battle,
                           Card& card)
{
    Fighter* invisibleMan = nullptr;

    if(attacker != nullptr && attacker->getName() == "InvisibleMan")
    {
        invisibleMan = attacker;
    }

    if(invisibleMan == nullptr)
    {
        cout << "Invisible Man is not available.\n";
        return;
    }

    vector<FogToken>& fogs = battle->getfogtoken();

    if(fogs.empty())
    {
        cout << "No Fog Tokens available on the board.\n";
        return;
    }

    cout << "\n--- Lurk Effect: Choose a Fog Token to move ---\n";

    for(int i = 0; i < fogs.size(); i++)
    {
        cout << i + 1<< ") Fog Token " << i + 1 << " (Zone " << fogs[i].getPosition()->getId() << ")\n";
    }

    int fogChoice = readInt("Choose Fog Token (0 to cancel): ",   0,   fogs.size());

    if(fogChoice == 0)
    {
        cout << "Action cancelled.\n";
        return;
    }

    fogChoice--;

    FogToken& selectedFog = fogs[fogChoice];

    vector<int> validIds;

    cout << "Valid destination zones: ";

    for(int id = 1; id <= 32; id++)
    {
        Zone* z = battle->getMap().getZone(id);

        if(z == nullptr)
            continue;

        // 1) No fighter in this zone
        Fighter* occupant = battle->getfighterat(z);

        if(occupant != nullptr)
            continue;

        // 2) No other Fog Token in this zone
        bool occupiedByOtherFog = false;

        for(int j = 0; j < fogs.size(); j++)
        {
            if(j == fogChoice)
                continue;

            if(fogs[j].getPosition() == z)
            {
                occupiedByOtherFog = true;
                break;
            }
        }

        if(occupiedByOtherFog)
            continue;

        // 3) Don't choose Invisible Man's current zone
        if(invisibleMan->getPosition() == z)
            continue;

        validIds.push_back(id);

        cout << id << " ";
    }

    cout << "\n";

    if(validIds.empty())
    {
        cout << "No valid destination zones available for this Fog Token.\n";
        return;
    }


    int destId = readchoice( "Enter destination zone ID (0 to cancel): ", validIds  );

    if(destId == 0)
    {
        cout << "Action cancelled.\n";
        return;
    }

    Zone* targetZone = battle->getMap().getZone(destId);

    if(targetZone == nullptr)
    {
        cout << "Zone not found.\n";
        return;
    }

   
    selectedFog.setPosition(targetZone);

    cout << "Fog Token moved to Zone " << targetZone->getId() << ".\n";


    invisibleMan->setPosition(targetZone);

    cout << "Invisible Man teleported to Zone "
         << targetZone->getId()
         << " along with the Fog Token!\n";
}


void SteplightlyEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card)
{
    // ۱. پیدا کردن خانه فعلی مرد نامرئی
    Fighter* invisibleMan = nullptr;
    if (attacker != nullptr && attacker->getName() == "InvisibleMan")
    {
        invisibleMan = attacker;
    }

    Zone* currentZone = invisibleMan->getPosition();
    if (currentZone == nullptr) return;

    cout << "\n--- Step Lightly Effect ---\n";
    cout << "Invisible Man is at Zone " << currentZone->getId() << ".\n";

    // ۲. بررسی تمام خانه‌های متصل (همسایه) برای پیدا کردن مبارزان حریف
    vector<Zone*> neighbors = currentZone->getNei();
    bool enemyFound = false;

    cout << "Scanning adjacent zones for enemies...\n";

    for (Zone* z : neighbors)
    {
        Fighter* occupant = battle->getfighterat(z);

        // اگر مبارزی در این خانه بود و دشمن بود
        if (occupant != nullptr && occupant->isalive() && occupant->getteam() != invisibleMan->getteam())
        {
            enemyFound = true;
            cout << "\nFound Enemy: " << occupant->getName() << " in Zone " << z->getId() << "!\n";

            // ۳. حالا بررسی می‌کنیم آیا مرد نامرئی روی توکن مه قرار دارد یا خیر
            bool isOnFog = false;
            vector<FogToken>& fogs = battle->getfogtoken();
            
            for (int i = 0; i < fogs.size(); i++)
            {
                if (fogs[i].getPosition() == currentZone)
                {
                    isOnFog = true;
                    break;
                }
            }
            // ۴. تعیین مقدار آسیب بر اساس وجود توکن مه
            int damageToDeal = isOnFog ? 3 : 1;
            
            if (isOnFog)
            {
                cout << "Invisible Man is on a Fog Token! Damage to deal is " << damageToDeal << ".\n";
            }
            else
            {
                cout << "Invisible Man iis NOT on a Fog Token. Damage to deal is " << damageToDeal << ".\n";
            }

            // اعمال آسیب به حریف پیدا شده
            occupant->takeDamage(damageToDeal);
            cout << occupant->getName() << " takes " << damageToDeal << " damage.\n";
        }
    }

    if (!enemyFound)
    {
        cout << "No enemies found in adjacent zones. No damage dealt.\n";
    }
}

void VanishEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card)
{
    Fighter* invisibleMan = nullptr;
    if (attacker != nullptr && attacker->getName() == "InvisibleMan")
    {
        invisibleMan = attacker;
    }

    invisibleMan->heal(1); 
    cout << "Invisible Man recovers 1 health! Current HP: " << invisibleMan->getHealth() << "\n";

    invisibleMan->setPosition(nullptr);
    cout << "Invisible Man has vanished from the board!\n";

}