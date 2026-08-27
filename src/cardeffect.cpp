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

    void FeastEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int guiChoice)
    {
        attacker->heal(2);
        cout << "Feast: Dracula healed +2 HP.\n";

        vector<Sisters>& sisters = battle->getsisters();
        vector<int> deadindexes;

        for (int i = 0; i < (int)sisters.size(); i++) 
        {
            if (!sisters[i].isalive()) 
            {
                deadindexes.push_back(i);
            }
        }

        if (deadindexes.empty()) 
        {
            cout << "Feast: No dead sister to revive.\n";
            return;
        }
        
        vector<Zone*> validZones;

        Fighter& dracula = battle->getDracual();

        vector<Zone*> zones = battle->getMap().getplacementZone(dracula.getPosition());

        for (Zone* z : zones) 
        {
            if (z != nullptr && battle->getfighterat(z) == nullptr) 
            {
                validZones.push_back(z);
            }
        }

        if (validZones.empty()) 
        {
            cout << "Feast: No valid empty zone of the same color available to revive sister.\n";
            return;
        }

        int idx = deadindexes[0];
        Fighter& revived = sisters[idx];
        revived.heal(1);

        Zone* targetZone = validZones[0]; 
        revived.setPosition(targetZone);

        cout << "Feast: " << revived.getName() << " revived with 1 HP at zone " << targetZone->getId() << " (shares color with Dracula).\n";
    }

   bool MistFormEffect::needsGUIInput() const { return true; }

    std::vector<int> MistFormEffect::getValidZones(Fighter* attacker, Battle* battle) const
    {
        std::vector<int> validIds;
        if (battle == nullptr) return validIds;

        Fighter& dracula = battle->getDracual();

        for (int id = 1; id <= 32; id++) {
            Zone* z = battle->getMap().getZone(id);
            if (z == nullptr) continue;
            Fighter* occ = battle->getfighterat(z);
            if (occ == nullptr || occ == &dracula) {
                validIds.push_back(id);
            }
        }
        return validIds;
    }

    void MistFormEffect::apply(Fighter* attacker,  Fighter* defender, Battle* battle, Card& card, int guiChoice)
    {
        if (battle == nullptr || guiChoice == -1) return;

        Fighter& dracula = battle->getDracual();
        Zone* targetZone = battle->getMap().getZone(guiChoice);
        if (targetZone == nullptr) return;

        Fighter* occ = battle->getfighterat(targetZone);
        if (occ != nullptr && occ != &dracula) return;

        if (targetZone != dracula.getPosition()) {
            dracula.setPosition(targetZone);
            cout << "Dracula moved to zone " << targetZone->getId() << endl;
        } else {
            cout << "Dracula stayed in zone " << targetZone->getId() << endl;
        }

        battle->giveExtraAction();
    }

    bool MonesterFormEffect::needsGUIInput() const { return true; }
    bool MonesterFormEffect::usesHandSelection() const { return true; }
    bool MonesterFormEffect::handSelectionRepeats() const { return true; }

       void MonesterFormEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int guiChoice) {
        if (guiChoice >= 0 && guiChoice < attacker->gethand().size()) {
            card.setValue(card.getValue() + 1);

            std::vector<Card>& hand = attacker->gethand();
            hand.erase(hand.begin() + guiChoice);

            std::cout << "Card discarded via GUI. New Attack Value: " << card.getValue() << "\n";
        }
    }
   bool ManeuverEffect::needsGUIInput() const { return true; }
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

                bool canPass = (occupant == nullptr || occupant->getteam() == attacker->getteam());

                if (canPass) {
                    visited.insert(neighbor);
                    q.push({neighbor, steps + 1});

                    if (occupant == nullptr) {
                        validIds.push_back(neighbor->getId());
                    }
                }
            }
        }

        return validIds;
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

    bool SeductiveCallEffect::needsMoreInput() const { return selectedCard != nullptr; }

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
        return false; 
    }
    bool SurvivalInstinctEffect::needsMoreInput() const { return selectedOpponent != nullptr; }
    bool SurvivalInstinctEffect::needsPostCombatGUI() const { return awaitingSelection; }
    bool SurvivalInstinctEffect::highlightAsTargetSelection() const { return selectedOpponent == nullptr; }

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
        if (guiChoice == -1) {
            if (selectedOpponent != nullptr) return;

            int finalAttack = card.getValue();
            int finalDefense = battle->getCombat()->getlastdefend();
            if (finalAttack > finalDefense) {
                awaitingSelection = true;
            }
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

        battle->getDracual().setPosition(finalZone);

        selectedOpponent = nullptr;
        awaitingSelection = false;
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
  void FixedPointEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int guiChoice)
{
    if (battle == nullptr) return;

    Fighter& sherlock = battle->getSherlock();
    Fighter& watson = battle->getWatson();

    if (!sherlock.isalive() || !watson.isalive() ||
        sherlock.getPosition() == nullptr || watson.getPosition() == nullptr)
    {
        cout << "Watson is not adjacent.\n";
        return;
    }

    bool adjacent = false;
    for (Zone* z : sherlock.getPosition()->getNei()) {
        if (z == watson.getPosition()) { adjacent = true; break; }
    }

    if (adjacent) {
        sherlock.heal(1);
        watson.heal(1);
        cout << "Fixed Point activated!\n";
    } else {
        cout << "Watson is not adjacent.\n";
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

     
    bool StudyMethodEffect::needsPostCombatGUI() const { return canViewHand; }
bool StudyMethodEffect::postCombatUsesHandDisplay() const { return true; }

void StudyMethodEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int guiChoice)
{
    canViewHand = false;
    if (attacker == nullptr || defender == nullptr || battle == nullptr) return;

    int dmg = battle->getCombat()->getFinalAttackValue() - battle->getCombat()->getFinalDefendValue();
    bool ownerIsAttacker = (attacker == battle->getCombat()->getCombatAttacker());
    bool won = ownerIsAttacker ? (dmg > 0) : (dmg <= 0);

    if (won) canViewHand = true;
}


    // ---------------- Elementary ----------------
   void ElementaryEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int guiChoice)
{
    if (guiChoice < 1 || guiChoice > 6 || battle == nullptr) return;

    int realAttack = battle->getCombat()->gelastattackcard().getValue();

    if (guiChoice == realAttack)
    {
        card.setValue(card.getValue() + 2);
        cout << "Correct prediction! Attacker's card is nullified.\n";

        Card* attackCard = battle->getCombat()->getCurrentAttackCard();
        if (attackCard != nullptr) {
            attackCard->forceValue(0);
            attackCard->lockValue();
        }
        battle->getCombat()->setCancel(true);
    }
    else
    {
        cout << "Wrong prediction\n";
    }
}

    // ---------------- Impossible ----------------
    bool ImpossibleEffect::needsGUIInput() const { return true; }
    bool ImpossibleEffect::usesHandSelection() const { return true; }
    Fighter* ImpossibleEffect::getHandSelectionTarget(Fighter* attacker, Fighter* defender) const { return defender; }

    void ImpossibleEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int guiChoice)
    {
        if (defender == nullptr) return;
        vector<Card>& enemyHand = defender->gethand();
        if (guiChoice < 0 || guiChoice >= (int)enemyHand.size()) return;

        cout << enemyHand[guiChoice].getName() << " was burned.\n";
        enemyHand.erase(enemyHand.begin() + guiChoice);
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

   bool StrategicDeductionEffect::needsGUIInput() const { return true; }
    bool StrategicDeductionEffect::usesHandSelection() const { return true; }
    Fighter* StrategicDeductionEffect::getHandSelectionTarget(Fighter* attacker, Fighter* defender) const { return defender; }

    void StrategicDeductionEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int guiChoice)
    {
        if (defender == nullptr || guiChoice < 0) return;
        vector<Card>& hand = defender->gethand();
        if (guiChoice >= (int)hand.size()) return;

        Card& enemyCard = hand[guiChoice];
        enemyCard.setBoost(enemyCard.getValue());
        cout << "Strategic Deduction: " << enemyCard.getName() << " boost changed to " << enemyCard.getValue() << endl;
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

    bool DeceptionEffect::needsGUIInput() const { return true; }
    bool DeceptionEffect::usesHandSelection() const { return true; }
    Fighter* DeceptionEffect::getHandSelectionTarget(Fighter* attacker, Fighter* defender) const { return defender; }

    void DeceptionEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int guiChoice)
    {
        if (attacker == nullptr || defender == nullptr) return;

        vector<Card>& enemyHand = defender->gethand();
        if (guiChoice < 0 || guiChoice >= (int)enemyHand.size()) return;

        cout << enemyHand[guiChoice].getName() << " was burned.\n";
        enemyHand.erase(enemyHand.begin() + guiChoice);
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

            if (guiChoice != -1)
            {
            Zone* targetZone = battle->getMap().getZone(guiChoice);

            if (targetZone != nullptr)
            {
                bool occupiedByAnotherFog = false;

                for (FogToken& fog : battle->getfogtoken())
                {
                    if (&fog != selectedFog &&
                        fog.getPosition() == targetZone)
                    {
                        occupiedByAnotherFog = true;
                        break;
                    }
                }

                if (occupiedByAnotherFog)
                {
                    std::cout << "There is already a Fog Token on this Zone.\n";
                    return;
                }

                selectedFog->setPosition(targetZone);
            }

            else {
                        std::cout << "Fog Token was not moved.\n";
                    }
        }
    }
   bool CodedNotesEffect::needsGUIInput() const { return true; }
bool CodedNotesEffect::usesHandSelection() const { return true; }
bool CodedNotesEffect::handSelectionRepeats() const { return pickedCount < 2; }
bool CodedNotesEffect::allowsSkip() const { return false; }

void CodedNotesEffect::onHandSelectionStart(Fighter* attacker, Fighter* defender, Battle* battle)
{
    if (attacker == nullptr) return;
    pickedCount = 0;
    for (int i = 0; i < 3; i++) {
        Card c = attacker->drawTopCard();
        if (c.getName() == "") break;
        attacker->gethand().push_back(c);
    }
}

void CodedNotesEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int guiChoice)
{
    if (attacker == nullptr || guiChoice < 0) return;
    vector<Card>& hand = attacker->gethand();
    if (guiChoice >= (int)hand.size()) return;

    Card picked = hand[guiChoice];
    hand.erase(hand.begin() + guiChoice);
    pickedCount++;

    if (pickedCount == 1) {
        firstPicked = picked;
        if (hand.empty()) {
            attacker->putCardOnTop(firstPicked);
            pickedCount = 2;
        }
        return;
    }

    attacker->putCardOnTop(firstPicked);
    attacker->putCardOnTop(picked);
}

   bool ConfoundEffect::needsGUIInput() const { return true; }
bool ConfoundEffect::usesHandSelection() const { return stage == Stage::DISCARD_CHOICE; }
bool ConfoundEffect::allowsSkip() const { return stage == Stage::DISCARD_CHOICE; }
bool ConfoundEffect::finishesOnSkip() const { return false; }
bool ConfoundEffect::needsMoreInput() const { return stage == Stage::FOG_DESTINATION; }
Fighter* ConfoundEffect::getHandSelectionTarget(Fighter*, Fighter* defender) const { return defender; }

void ConfoundEffect::onSkip(Fighter*, Fighter*, Battle*)
{
    stage = Stage::FOG_SELECT;
}

std::vector<int> ConfoundEffect::getValidZones(Fighter* attacker, Battle* battle) const
{
    std::vector<int> validIds;
    if (battle == nullptr) return validIds;
    auto& fogs = battle->getfogtoken();

    if (stage == Stage::FOG_SELECT) {
        for (auto& fog : fogs)
            if (fog.getPosition() != nullptr) validIds.push_back(fog.getPosition()->getId());
    } else if (stage == Stage::FOG_DESTINATION) {
        for (int id = 1; id <= 32; id++) {
            Zone* z = battle->getMap().getZone(id);
            if (z == nullptr) continue;
            bool occupiedByOtherFog = false;
            for (auto& fog : fogs)
                if (&fog != selectedFog && fog.getPosition() == z) { occupiedByOtherFog = true; break; }
            if (!occupiedByOtherFog) validIds.push_back(id);
        }
    }
    return validIds;
}

void ConfoundEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int guiChoice)
{
    if (attacker == nullptr || battle == nullptr || guiChoice == -1) return;

    if (stage == Stage::DISCARD_CHOICE) {
        if (defender != nullptr) {
            vector<Card>& hand = defender->gethand();
            if (guiChoice < (int)hand.size()) hand.erase(hand.begin() + guiChoice);
        }
        return;
    }

    auto& fogs = battle->getfogtoken();
    if (fogs.empty()) { stage = Stage::DISCARD_CHOICE; selectedFog = nullptr; return; }

    if (stage == Stage::FOG_SELECT) {
        for (auto& fog : fogs)
            if (fog.getPosition() != nullptr && fog.getPosition()->getId() == guiChoice) { selectedFog = &fog; break; }
        stage = Stage::FOG_DESTINATION;
        return;
    }

    Zone* targetZone = battle->getMap().getZone(guiChoice);
    if (targetZone != nullptr) {
        bool occupied = false;
        for (FogToken& fog : fogs) if (&fog != selectedFog && fog.getPosition() == targetZone) { occupied = true; break; }
        if (!occupied) selectedFog->setPosition(targetZone);
    }
    selectedFog = nullptr;
    stage = Stage::DISCARD_CHOICE;
}

    bool CovertPreparationEffect::needsGUIInput() const {

    return true;
    }

    bool CovertPreparationEffect::needsMoreInput() const { return selectedFog != nullptr; }

    std::vector<int> CovertPreparationEffect::getValidZones(Fighter* attacker, Battle* battle) const {
        std::vector<int> validIds;
        if (battle == nullptr) return validIds;
        auto& fogs = battle->getfogtoken();

        if (selectedFog == nullptr) {
            for (auto& fog : fogs)
                if (fog.getPosition() != nullptr) validIds.push_back(fog.getPosition()->getId());
        } else {
            auto reachable = battle->getBoard()->getReachableZoneFromZone(selectedFog->getPosition(), 2);
            for (Zone* z : reachable) {
                bool occupiedByOtherFog = false;
                for (auto& fog : fogs)
                    if (&fog != selectedFog && fog.getPosition() == z) { occupiedByOtherFog = true; break; }
                if (!occupiedByOtherFog) validIds.push_back(z->getId());
            }
        }
        return validIds;
    }

    void CovertPreparationEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int guiChoice)
    {
        if (attacker == nullptr || battle == nullptr) return;

        if (selectedFog == nullptr) {
            Card c = attacker->drawTopCard();
            if (c.getName() != "") { attacker->gethand().push_back(c); }

            auto& fogs = battle->getfogtoken();
            if (fogs.empty() || guiChoice == -1) return;

            for (auto& fog : fogs)
                if (fog.getPosition() != nullptr && fog.getPosition()->getId() == guiChoice) { selectedFog = &fog; break; }
            return;   
        }

        if (guiChoice != -1) {
            Zone* targetZone = battle->getMap().getZone(guiChoice);
            if (targetZone != nullptr) selectedFog->setPosition(targetZone);
        }
        selectedFog = nullptr;
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
        if (fog.getPosition() != nullptr && battle->getfighterat(fog.getPosition()) == nullptr) {
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
    bool RollingFogEffect::needsMoreInput() const { return selectedFog != nullptr; }

    std::vector<int> RollingFogEffect::getValidZones(Fighter* attacker, Battle* battle) const {
        std::vector<int> validIds;
        if (battle == nullptr) return validIds;
        auto& fogs = battle->getfogtoken();

        if (selectedFog == nullptr) {
            validIds.push_back(0);
            for (const auto& fog : fogs)
                if (fog.getPosition() != nullptr) validIds.push_back(fog.getPosition()->getId());
        } else {
            for (int id = 1; id <= 32; id++) {
                Zone* z = battle->getMap().getZone(id);
                if (z == nullptr) continue;
                bool occupiedByOtherFog = false;
                for (const auto& fog : fogs)
                    if (&fog != selectedFog && fog.getPosition() == z) { occupiedByOtherFog = true; break; }
                if (!occupiedByOtherFog) validIds.push_back(id);
            }
        }
        return validIds;
    }

    void RollingFogEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int guiChoice)
    {
        if (battle == nullptr || guiChoice == -1) return;
        auto& fogs = battle->getfogtoken();
        if (fogs.empty()) return;

        if (selectedFog == nullptr) {
            if (guiChoice == 0) {
                battle->giveExtraAction();
                std::cout << "Extra action gained!\n";
                return;
            }
            for (auto& fog : fogs)
                if (fog.getPosition() != nullptr && fog.getPosition()->getId() == guiChoice) { selectedFog = &fog; break; }
            return;   
        }

        Zone* targetZone = battle->getMap().getZone(guiChoice);
    if (targetZone != nullptr)
    {
        bool occupiedByAnotherFog = false;

        for (FogToken& fog : battle->getfogtoken())
        {
            if (&fog != selectedFog &&
                fog.getPosition() == targetZone)
            {
                occupiedByAnotherFog = true;
                break;
            }
        }

        if (occupiedByAnotherFog)
        {
            std::cout << "There is already a Fog Token on this Zone.\n";
            return;
        }

        selectedFog->setPosition(targetZone);

        std::cout << "Fog Token moved to Zone "
                << guiChoice << std::endl;
    }
        selectedFog = nullptr;
    }

    bool SlipAwayEffect::needsGUIInput() const {
        return true;
    }

    bool SlipAwayEffect::needsMoreInput() const {
        return selectedFogIndex != -1;
    }

    std::vector<int> SlipAwayEffect::getValidZones(Fighter* attacker, Battle* battle) const {
        std::vector<int> validIds;
        if (attacker == nullptr || battle == nullptr) return validIds;

        std::vector<FogToken>& fogs = battle->getfogtoken();

        if (selectedFogIndex == -1) {
            for (int i = 0; i < fogs.size(); i++) {
                if (fogs[i].getPosition() != nullptr) {
                    validIds.push_back(fogs[i].getPosition()->getId());
                }
            }
        } else {
            for (int id = 1; id <= 32; id++) {
                Zone* z = battle->getMap().getZone(id);
                if (z == nullptr) continue;

                if (battle->getfighterat(z) != nullptr) continue;

                bool occupiedByOtherFog = false;
                for (int j = 0; j < fogs.size(); j++) {
                    if (j == selectedFogIndex) continue;
                    if (fogs[j].getPosition() == z) {
                        occupiedByOtherFog = true;
                        break;
                    }
                }
                if (occupiedByOtherFog) continue;

                if (attacker->getPosition() == z) continue;

                validIds.push_back(id);
            }
        }
        return validIds;
    }

    void SlipAwayEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int guiChoice) {
        if (attacker == nullptr || battle == nullptr || guiChoice == -1) return;

        std::vector<FogToken>& fogs = battle->getfogtoken();
        if (fogs.empty()) return;

        if (selectedFogIndex == -1) {
            for (int i = 0; i < fogs.size(); i++) {
                if (fogs[i].getPosition() != nullptr && fogs[i].getPosition()->getId() == guiChoice) {
                    selectedFogIndex = i;
                    break;
                }
            }
            return; 
        }

        Zone* targetZone = battle->getMap().getZone(guiChoice);
        if (targetZone != nullptr) {
            fogs[selectedFogIndex].setPosition(targetZone);
            attacker->setPosition(targetZone);
        }

        selectedFogIndex = -1;
    }

    bool SteplightlyEffect::needsGUIInput() const {
        return true;   
    }

    bool SteplightlyEffect::needsMoreInput() const {
        return selectedFog != nullptr; 
    }

    std::vector<int> SteplightlyEffect::getValidZones(Fighter* attacker, Battle* battle) const {
        std::vector<int> validIds;
        if (battle == nullptr) return validIds;
        auto& fogs = battle->getfogtoken();

        if (selectedFog == nullptr) {
            for (auto& fog : fogs) {
                if (fog.getPosition() != nullptr) {
                    validIds.push_back(fog.getPosition()->getId());
                }
            }
        } else {
            auto reachable = battle->getBoard()->getReachableZoneFromZone(selectedFog->getPosition(), 2);
            for (Zone* z : reachable) {
                bool occupiedByOtherFog = false;
                for (auto& fog : fogs) {
                    if (&fog != selectedFog && fog.getPosition() == z) {
                        occupiedByOtherFog = true;
                        break;
                    }
                }
                if (!occupiedByOtherFog) {
                    validIds.push_back(z->getId());
                }
            }
        }
        return validIds;
    }

    void SteplightlyEffect::apply(Fighter* attacker, Fighter* defender, Battle* battle, Card& card, int guiChoice) 
    {
        if (attacker == nullptr || battle == nullptr) return;

        if (!damageDealt) 
        {
            Fighter* invisibleMan = nullptr;
            if (attacker->getName() == "InvisibleMan") 
            {
                invisibleMan = attacker;
            }

            if (invisibleMan != nullptr) {
                Zone* currentZone = invisibleMan->getPosition();
                if (currentZone != nullptr) {
                    cout << "\n--- Step Lightly Effect ---\n";
                    cout << "Invisible Man is at Zone " << currentZone->getId() << ".\n";

                    bool isOnFog = false;
                    vector<FogToken>& fogs = battle->getfogtoken();
                    
                    for (int i = 0; i < fogs.size(); i++) {
                        if (fogs[i].getPosition() == currentZone) 
                        {
                            isOnFog = true;
                            break;
                        }
                    }
                    
                    int damageToDeal = isOnFog ? 3 : 1;
                    
                    if (isOnFog) {
                        cout << "Invisible Man is on a Fog Token! Damage to deal is " << damageToDeal << ".\n";
                    } else {
                        cout << "Invisible Man is NOT on a Fog Token. Damage to deal is " << damageToDeal << ".\n";
                    }

                    vector<Zone*> neighbors = currentZone->getNei();
                    bool enemyFound = false;

                    cout << "Scanning adjacent zones for enemies...\n";
                    for (Zone* z : neighbors) {
                        Fighter* occupant = battle->getfighterat(z);
                        if (occupant != nullptr && occupant->isalive() && occupant->getteam() != invisibleMan->getteam()) {
                            enemyFound = true;
                            cout << "\nFound Enemy: " << occupant->getName() << " in Zone " << z->getId() << "!\n";
                            occupant->takeDamage(damageToDeal);
                            cout << occupant->getName() << " takes " << damageToDeal << " damage.\n";
                        }
                    }

                    if (!enemyFound) {
                        cout << "No enemies found in adjacent zones. No damage dealt.\n";
                    }
                }
            }
            damageDealt = true; 
        }

        auto& fogs = battle->getfogtoken();
        if (fogs.empty()) return;

        if (selectedFog == nullptr) {
            if (guiChoice == -1) return; 
            
            for (auto& fog : fogs) {
                if (fog.getPosition() != nullptr && fog.getPosition()->getId() == guiChoice) {
                    selectedFog = &fog;
                    break;
                }
            }
            return;
        }

        if (guiChoice != -1) {
            Zone* targetZone = battle->getMap().getZone(guiChoice);
            if (targetZone != nullptr) {
                selectedFog->setPosition(targetZone);
                cout << "Opponent moved Fog Token to Zone " << guiChoice << ".\n";
            }
        }
        
        selectedFog = nullptr;
        damageDealt = false;
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