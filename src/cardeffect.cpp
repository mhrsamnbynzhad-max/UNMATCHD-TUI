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
#include <queue>
#include <set>
using namespace std;

void BloodThirstEffect :: apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card , int guiChoice)
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
       
void AmbushEffect :: apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card , int guiChoice)
{
        if(!defender || defender->handsize() == 0)
        {
            return;
        }

        int  index = rand() % defender->handsize();  
        Card remove = defender->remove_ranodmcard();

        int opponentBoost = remove.getBoost();
        card.setValue(card.getValue() +  opponentBoost);


}

 void FeastEffect :: apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card , int guiChoice)
{
        attacker->heal(2);
       // cout<<"Feast: Dracula healed +2 HP. \n";

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

void MistFormEffect::apply(Fighter* attacker,  Fighter* defender, Battle* battle, Card& card, int guiChoice)
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
 void MonesterFormEffect :: apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card , int guiChoice) 
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

   bool ManeuverEffect::needsGUIInput() const {
    return true;
}
void ManeuverEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int guiChoice) {
    if (guiChoice == -1 || attacker == nullptr) return; 

    std::vector<int> validZones = getValidZones(attacker, battle);
    bool isValid = false;
    for (int id : validZones) {
        if (id == guiChoice) {
            isValid = true;
            break;
        }
    }

    if (!isValid) return;  

    Zone* selectedZone = battle->getMap().getZone(guiChoice);
    if (selectedZone != nullptr) {
        attacker->setPosition(selectedZone);
    }
}



std::vector<int> ManeuverEffect::getValidZones(Fighter* attacker, Battle* battle) const {
    std::vector<int> validIds;
    if (attacker == nullptr || battle == nullptr) return validIds;

    Zone* startZone = attacker->getPosition();
    if (startZone == nullptr) return validIds;

    std::set<Zone*> visited;
    std::queue<std::pair<Zone*, int>> q;

    q.push({startZone, 0});
    visited.insert(startZone);

    while (!q.empty()) {
        auto [currentZone, steps] = q.front();
        q.pop();

        if (steps >= 3) continue;

        for (Zone* neighbor : currentZone->getNei()) {
            if (neighbor == nullptr) continue;
            if (visited.count(neighbor)) continue; 

            Fighter* occupant = battle->getfighterat(neighbor);

            bool canPassOrStop = (occupant == nullptr || occupant->getteam() == attacker->getteam());

            if (canPassOrStop) {
                visited.insert(neighbor);
                q.push({neighbor, steps + 1});
                
                validIds.push_back(neighbor->getId());
            }
        }
    }

    return validIds;
}
  void  ExploitEffect :: apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card , int guiChoice) 
  {
    auto cards = attacker->getrandomcard(1);
    if(!cards.empty())
    {
        attacker->addtohand(cards);
        cout<<"Draw a card . added to your hand .\n";
    }
  }

  void LookIntoMyEyesEffect :: apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card,int guuichoice  )
  {
    int oppBoost = battle->getCombat()->gelastattackcard().getBoost();

    card.setValue(card.getValue() + oppBoost);

    cout<<" opponent  boost ("<<oppBoost<<" 0 added to this defense.\n";
  }

void HuntEyesEffect :: apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card ,int guuichoice = -1) 
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


bool SeductiveCallEffect::needsGUIInput() const 
{
    return true; 
}

std::vector<int> SeductiveCallEffect::getValidZones(Fighter* attacker, Battle* battle) const
{
    std::vector<int> validIds;
    if (battle == nullptr) return validIds;

    if (selectedCard == nullptr) {
        for (Fighter* f : battle->getFighters()) {
            if (f != nullptr && f != attacker && f->isalive() && f->getPosition() != nullptr) 
            {
                validIds.push_back(f->getPosition()->getId());
            }
        }
    } else {
        vector<Zone*> reachableZones = battle->getBoard()->getReachableZone(*selectedCard, 2);
        for (Zone* z : reachableZones) {
            Fighter* occ = battle->getfighterat(z);
            if (occ == nullptr || occ == selectedCard) {
                validIds.push_back(z->getId());
            }
        }
    }
    return validIds;
}

void SeductiveCallEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int guiChoice) {
    if (guiChoice == -1) return;

    if (selectedCard == nullptr) {
        Zone* clickedZone = battle->getMap().getZone(guiChoice);
        if (clickedZone != nullptr) {
            selectedCard = battle->getfighterat(clickedZone);
        }
        return; 
    }

    Zone* targetZone = battle->getMap().getZone(guiChoice);
    if (targetZone == nullptr) return;

    selectedCard->setPosition(targetZone);

    if (selectedCard->getName() != "Sister") {
        vector<Zone*> connectedToTarget = targetZone->getNei();
        int sisterCount = 0;

        for (Zone* z : connectedToTarget) {
            Fighter* occ = battle->getfighterat(z);
            if (occ != nullptr && occ->getName() == "Sister") {
                sisterCount++;
            }
        }

        if (sisterCount > 0) {
            int totalDamage = sisterCount * 1;
            selectedCard->takeDamage(totalDamage);
        }
    }

    selectedCard = nullptr;
}


bool SurvivalInstinctEffect::needsGUIInput() const {
    return true; 
}

std::vector<int> SurvivalInstinctEffect::getValidZones(Fighter* attacker, Battle* battle) const {
    std::vector<int> validIds;
    if (battle == nullptr) return validIds;

    if (selectedOpponent == nullptr) {
        for (Fighter* f : battle->getFighters()) {
            if (f != nullptr && f->getteam() != attacker->getteam() && f->getPosition() != nullptr) {
                validIds.push_back(f->getPosition()->getId());
            }
        }
    } 
    else {
        Zone* opponentZone = selectedOpponent->getPosition();
        if (opponentZone != nullptr) {
            for (Zone* z : opponentZone->getNei()) {
                if (z != nullptr && battle->getfighterat(z) == nullptr) {
                    validIds.push_back(z->getId());
                }
            }
        }
    }
    return validIds;
}

void SurvivalInstinctEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int guiChoice) {
    if (guiChoice == -1) return;

    int finalAttack = card.getValue();
    int finalDefense = battle->getCombat()->getlastdefend();
    if (finalAttack <= finalDefense) {
        return; 
    }

    if (selectedOpponent == nullptr) {
        Zone* clickedZone = battle->getMap().getZone(guiChoice);
        if (clickedZone != nullptr) {
            selectedOpponent = battle->getfighterat(clickedZone);
        }
        return; 
    }

    Zone* finalZone = battle->getMap().getZone(guiChoice);
    if (finalZone == nullptr) return;

 
    attacker->setPosition(finalZone); 

    selectedOpponent = nullptr;
}

 void FeintEffect :: apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card,int guuichoice )
 {
    battle->getCombat()->setCancel(true);
 }



// ---------------- Counter Attack ----------------

void CounterAttackEffect::apply(Fighter* attacker,    Fighter* defender,   Battle* battle,   Card& card,int guuichoice )
{
    if(!attacker || !defender)
        return;


    if(battle->getBoard()->areadjacent(*attacker,*defender))
    {
        defender->takeDamage(2);
    }
  
}



// ---------------- Fixed Point ----------------
void FixedPointEffect::apply(Fighter* attacker,  Fighter* defender,  Battle* battle,  Card& card,int guuichoice )
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

   bool ServiceEffect::needsGUIInput() const {
    return true;
}
std::vector<int> ServiceEffect::getValidZones(Fighter* attacker, Battle* battle) const {
    std::vector<int> validIds;
    if (attacker == nullptr || battle == nullptr) return validIds;

    Fighter* watson = nullptr;
    for (Fighter* f : battle->getFighters()) {
        if (f != nullptr && f->getName() == "Watson") {
            watson = f;
            break;
        }
    }

    if (watson == nullptr) return validIds;

    Zone* sherlockzone = attacker->getPosition();
    if (sherlockzone == nullptr) return validIds;

    for (Zone* z : sherlockzone->getNei()) {
        if (z != nullptr && battle->getfighterat(z) == nullptr) {
            validIds.push_back(z->getId());
        }
    }

    return validIds;
}

void ServiceEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int guiChoice) {
    if (attacker == nullptr || battle == nullptr) return;

    Fighter* watson = nullptr;
    for (Fighter* f : battle->getFighters()) {
        if (f != nullptr && f->getName() == "Watson") {
            watson = f;
            break;
        }
    }

    if (watson == nullptr) return;

    std::vector<int> validZones = getValidZones(attacker, battle);

    if (!validZones.empty()) {
        battle->getSherlock().heal(2);

        if (guiChoice != -1) {
            bool isValid = false;
            for (int id : validZones) {
                if (id == guiChoice) {
                    isValid = true;
                    break;
                }
            }

            if (isValid) {
                Zone* targetZone = battle->getMap().getZone(guiChoice);
                if (targetZone != nullptr) {
                    watson->setPosition(targetZone);
                }
            }
        }
    }

    attacker->heal(1);

    std::vector<Card> cards = attacker->getrandomcard(1);
    if (!cards.empty()) {
        attacker->addtohand(cards);
    }
}

// ---------------- Study Method ----------------

void StudyMethodEffect::apply(Fighter* attacker,    Fighter* defender,    Battle* battle,    Card& card,int guuichoice )
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

   bool ElementaryEffect::needsGUIInput() const {
    return false;
}

void ElementaryEffect::apply(Fighter* attacker,    Fighter* defender,    Battle* battle,    Card& card,int guuichoice )
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
void ImpossibleEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card,int guuichoice )
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

bool MasterOfDisguiseEffect::needsGUIInput() const {
    return false;
}

void MasterOfDisguiseEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int guiChoice) {
    Fighter* sherlock = nullptr;
    Fighter* opponent = nullptr;

    for (Fighter* f : battle->getFighters()) {
        if (f->getName() == "Sherlock")
            sherlock = f;
    }

    if (attacker != nullptr && attacker->getName() != "Sherlock") {
        opponent = attacker;
    } else if (defender != nullptr && defender->getName() != "Sherlock") {
        opponent = defender;
    }

    if (sherlock == nullptr || opponent == nullptr) {
        return;
    }

    Zone* sherlockZone = sherlock->getPosition();
    Zone* opponentZone = opponent->getPosition();

    if (sherlockZone == nullptr || opponentZone == nullptr) {
        return;
    }

    sherlock->setPosition(opponentZone);
    opponent->setPosition(sherlockZone);

    opponent->takeDamage(1);
}


// ---------------- Game On ----------------

bool GameOnEffect::needsGUIInput() const {
    return true;
}

std::vector<int> GameOnEffect::getValidZones(Fighter* attacker, Battle* battle) const {
    std::vector<int> validIds;
    if (attacker == nullptr || battle == nullptr) return validIds;

    std::vector<Zone*> reachable = battle->getBoard()->getReachableZone(*attacker, 3);
    for (Zone* z : reachable) {
        if (z != nullptr) {
            validIds.push_back(z->getId());
        }
    }

    return validIds;
}

void GameOnEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int guiChoice) {
    if (attacker == nullptr || battle == nullptr) return;

    if (guiChoice == -1) return;

    std::vector<int> validZones = getValidZones(attacker, battle);
    bool isValid = false;
    for (int id : validZones) {
        if (id == guiChoice) {
            isValid = true;
            break;
        }
    }

    if (!isValid) return;

    battle->getBoard()->movefighter(*attacker, guiChoice, 3);
}
// ---------------- Confirm Suspicion ----------------

void ConfirmSuspicionEffect::apply(Fighter* attacker, Fighter* defender,Battle* battle, Card& card, int guichoice)
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

void StrategicDeductionEffect::apply( Fighter* attacker, Fighter* defender, Battle* battle, Card& card , int guichoice)
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

void LearningNeverEndsEffect::apply( Fighter* attacker, Fighter* defender, Battle* battle, Card& card,int guiChoice)
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

void DeceptionEffect::apply(Fighter* attacker,Fighter* defender,Battle* battle,Card& card , int guiChoice)
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

void SidearmEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int guiChoice)
{

    if(!defender)
        return;

    defender->takeDamage(card.getValue() );
    cout<<"Sidearm deals "<<card.getValue()<<" damage.\n";
}

void movetoken(Battle* battle, int v, int guiChoice, FogToken* selectedFog)
{
    if (battle == nullptr || selectedFog == nullptr) return;

    if (guiChoice != -1) {
        if (guiChoice != 0) {
            Zone* targetZone = battle->getMap().getZone(guiChoice);
            if (targetZone != nullptr) {
                selectedFog->setPosition(targetZone);
                std::cout << "Fog Token moved to Zone " << guiChoice << std::endl;
            }
        } else {
            std::cout << "Fog Token was not moved.\n";
        }
    }
}
void CodedNotesEffect::apply(Fighter* attacker,  Fighter* defender,  Battle* battle,  Card& card , int guiChoice)
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

void ConfoundEffect::apply(Fighter* attacker, Fighter* defender,   Battle* battle,   Card& card , int guiChoice)
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

  bool CovertPreparationEffect::needsGUIInput() const {
    return true; // فعال کردن ورودی گرافیکی برای این کارت
}

std::vector<int> CovertPreparationEffect::getValidZones(Fighter* attacker, Battle* battle) const {
    std::vector<int> validIds;
    if (battle == nullptr) return validIds;

    // می‌توانید بر اساس مرحله‌ی انتخاب (مثلا انتخاب مقصد مه یا زون دشمن) لیست زون‌های معتبر را برگردانید
    vector<FogToken>& fogs = battle->getfogtoken();
    for (const auto& fog : fogs) {
        if (fog.getPosition() != nullptr) {
            validIds.push_back(fog.getPosition()->getId());
        }
    }
    return validIds;
}

void CovertPreparationEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int guiChoice)
{
    if (attacker == nullptr || battle == nullptr) return;

    // 1) کشیدن یک کارت (این بخش بدون تغییر و اتوماتیک انجام می‌شود)
    Card c = attacker->drawTopCard();
    if (c.getName() != "")
    {
        attacker->gethand().push_back(c);
        cout << "Drew card : " << c.getName() << endl;
    }

    vector<FogToken>& fogs = battle->getfogtoken();
    if (fogs.empty()) return;

    // انتخاب پیش‌فرض یا بر اساس guiChoice ورودی‌داده‌شده از گرافیک
    int fogChoiceIndex = 0; 
    FogToken& selectedFog = fogs[fogChoiceIndex];

    // 3) جابجایی مه (اگر guiChoice داده شده باشد می‌توان از آن استفاده کرد)
    vector<Zone*> reachable = battle->getBoard()->getReachableZoneFromZone(selectedFog.getPosition(), 2);
    if (guiChoice != -1) {
        Zone* targetZone = battle->getMap().getZone(guiChoice);
        if (targetZone != nullptr) {
            selectedFog.setPosition(targetZone);
        }
    }

    Fighter* enemy = defender;
    if (!enemy) return;

    vector<FogToken*> otherFogs;
    for (size_t i = 0; i < fogs.size(); i++)
    {
        if (i != (size_t)fogChoiceIndex)
            otherFogs.push_back(&fogs[i]);
    }

    if (otherFogs.empty()) return;

    bool close = false;
    for (FogToken* fog : otherFogs)
    {
        vector<Zone*> zones = battle->getBoard()->getReachableZoneFromZone(fog->getPosition(), 2);
        for (Zone* z : zones)
        {
            if (z == enemy->getPosition())
            {
                close = true;
                break;
            }
        }
        if (close) break;
    }

    if (close)
    {
        cout << "Enemy is already near a fog.\n";
        return;
    }

    // 5) جابجایی دشمن نزدیک به نزدیک‌ترین مه
    cout << "Enemy is moved near Fog.\n";
    FogToken* targetFog = otherFogs[0];

    vector<Zone*> possible;
    vector<Zone*> around = battle->getMap().getplacementZone(targetFog->getPosition());

    for (Zone* z : around)
    {
        vector<Zone*> dist = battle->getBoard()->getReachableZoneFromZone(targetFog->getPosition(), 2);
        for (Zone* x : dist)
        {
            if (x == z && battle->getfighterat(z) == nullptr)
            {
                possible.push_back(z);
            }
        }
    }

    if (!possible.empty())
    {
        enemy->setPosition(possible[0]);
        cout << enemy->getName() << " moved near Fog Zone " << targetFog->getPosition()->getId() << endl;
    }
}
void DreamingOfRevengeEffect::apply(Fighter* attacker,  Fighter* defender,  Battle* battle,  Card& card , int guiChoice)
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

void EmergefrommistEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card , int guiChoice)
{
    if(attacker == nullptr || battle == nullptr)
        return;

    if(battle->startedTurnOnFog())
    {
        card.setValue(5);
    }
    
}

 void ImpossibletoseeEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card , int guiChoice)
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


bool IntoThinAirEffect::needsGUIInput() const {
    return true;
}

std::vector<int> IntoThinAirEffect::getValidZones(Fighter* attacker, Battle* battle) const {
    std::vector<int> validIds;
    if (attacker == nullptr || battle == nullptr) return validIds;

    validIds.push_back(0);
    Zone* currentPos = attacker->getPosition();
    if (currentPos != nullptr) {
        for (Zone* z : currentPos->getNei()) {
            if (z != nullptr && battle->getfighterat(z) == nullptr) {
                validIds.push_back(z->getId());
            }
        }
    }
    return validIds;
}

void IntoThinAirEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int guiChoice)
{
    if (attacker == nullptr || battle == nullptr) return;

    if (guiChoice != -1) {
        if (guiChoice != 0) {
            Zone* targetZone = battle->getMap().getZone(guiChoice);
            if (targetZone != nullptr && battle->getfighterat(targetZone) == nullptr) {
                attacker->setPosition(targetZone);
                std::cout << "InvisibleMan moved to " << guiChoice << std::endl;
            }
        } else {
            std::cout << "InvisibleMan stayed here.\n";
        }
        
        auto& fogs = battle->getfogtoken();
        if (!fogs.empty()) {
            movetoken(battle, 3, guiChoice, &fogs[0]);
        }
    }
}

bool LurkingEffect::needsGUIInput() const {
    return true;
}

std::vector<int> LurkingEffect::getValidZones(Fighter* attacker, Battle* battle) const {
    std::vector<int> validIds;
    if (attacker == nullptr || battle == nullptr) return validIds;

    auto& fogs = battle->getfogtoken();
    
    for (const auto& fog : fogs) {
        if (fog.getPosition() != nullptr) {
            validIds.push_back(fog.getPosition()->getId());
        }
    }
    
    return validIds;
}

void LurkingEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int guiChoice)
{
    if (attacker == nullptr || battle == nullptr) return;

    attacker->addtohand(attacker->getrandomcard(1));

    auto& fogs = battle->getfogtoken();
    if (fogs.empty()) return;


    if (guiChoice != -1) {
      
        FogToken* targetFog = nullptr;
        for (auto& fog : fogs) {
            if (fog.getPosition() != nullptr && fog.getPosition()->getId() == guiChoice) {
                targetFog = &fog;
                break;
            }
        }

        if (targetFog != nullptr) {
            attacker->setPosition(targetFog->getPosition());
            std::cout << "Invisible Man moved to Fog Token at Zone " << guiChoice << std::endl;
        } else {
            movetoken(battle, 3, guiChoice, &fogs[0]);
        }
    }
}

void ReignOfTerrorEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card , int guiChoice)
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

  bool RollingFogEffect::needsGUIInput() const {
    return true;
}

std::vector<int> RollingFogEffect::getValidZones(Fighter* attacker, Battle* battle) const {
    std::vector<int> validIds;
    if (battle == nullptr) return validIds;

    auto& fogs = battle->getfogtoken();


    validIds.push_back(0);
    for (const auto& fog : fogs) {
        if (fog.getPosition() != nullptr) {
            validIds.push_back(fog.getPosition()->getId());
        }
    }

    return validIds;
}

void RollingFogEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int guiChoice)
{
    if (battle == nullptr) return;

    auto& fogs = battle->getfogtoken();
    if (fogs.empty()) return;

    if (guiChoice != -1) {
        if (guiChoice == 0) {
            battle->giveExtraAction();
            std::cout << "Extra action gained!\n";
            return;
        }

        FogToken* selectedFog = nullptr;
        for (auto& fog : fogs) {
            if (fog.getPosition() != nullptr && fog.getPosition()->getId() == guiChoice) {
                selectedFog = &fog;
                break;
            }
        }

        if (selectedFog != nullptr) {
           
            std::vector<int> validZones;
            for (int id = 1; id <= 32; id++) {
                Zone* z = battle->getMap().getZone(id);
                if (z == nullptr) continue;

                bool occupiedByOtherFog = false;
                for (const auto& otherFog : fogs) {
                    if (&otherFog != selectedFog && otherFog.getPosition() == z) {
                        occupiedByOtherFog = true;
                        break;
                    }
                }
                if (!occupiedByOtherFog) {
                    validZones.push_back(id);
                }
            }

            Zone* targetZone = battle->getMap().getZone(guiChoice);
            if (targetZone != nullptr) {
                selectedFog->setPosition(targetZone);
                std::cout << "Fog moved to Zone " << guiChoice << std::endl;
            }
        }
    }
}

void SlipAwayEffect::apply(Fighter* attacker,
                           Fighter* defender,
                           Battle* battle,
                           Card& card, int guichoice) 
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



void SteplightlyEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card , int guiChoice)
{
    Fighter* invisibleMan = nullptr;
    if (attacker != nullptr && attacker->getName() == "InvisibleMan")
    {
        invisibleMan = attacker;
    }

    Zone* currentZone = invisibleMan->getPosition();
    if (currentZone == nullptr) return;

    cout << "\n--- Step Lightly Effect ---\n";
    cout << "Invisible Man is at Zone " << currentZone->getId() << ".\n";

    vector<Zone*> neighbors = currentZone->getNei();
    bool enemyFound = false;

    cout << "Scanning adjacent zones for enemies...\n";

    for (Zone* z : neighbors)
    {
        Fighter* occupant = battle->getfighterat(z);

        if (occupant != nullptr && occupant->isalive() && occupant->getteam() != invisibleMan->getteam())
        {
            enemyFound = true;
            cout << "\nFound Enemy: " << occupant->getName() << " in Zone " << z->getId() << "!\n";

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
            int damageToDeal = isOnFog ? 3 : 1;
            
            if (isOnFog)
            {
                cout << "Invisible Man is on a Fog Token! Damage to deal is " << damageToDeal << ".\n";
            }
            else
            {
                cout << "Invisible Man is NOT on a Fog Token. Damage to deal is " << damageToDeal << ".\n";
            }

            occupant->takeDamage(damageToDeal);
            cout << occupant->getName() << " takes " << damageToDeal << " damage.\n";
        }
    }

    if (!enemyFound)
    {
        cout << "No enemies found in adjacent zones. No damage dealt.\n";
    }
}

void VanishEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card , int guiChoice)
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