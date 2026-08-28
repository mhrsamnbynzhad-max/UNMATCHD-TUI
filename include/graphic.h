#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>
#include <map>
#include "battle.h"


#include <SFML/Audio.hpp>


struct Spot
{
    int id;
    sf::CircleShape circle;
};

class GraphicManager
{
private:

    sf::Music music;
    bool musicLoaded = false;
    sf::RenderWindow window;
    sf::Texture backgroundTexture;
    std::optional<sf::Sprite> background;
    
    std::vector<Spot> boardSpots;

     std::vector<sf::Texture> spotTextures;
    std::vector<std::pair<int, int>> edges;

     void initEdges(); 
    void initSpots();
 
enum class SetupState {
    MAIN_MENU,

    AGE_P1,
    AGE_P2,
    HERO_1,
    HERO_2,

    SIDEKICK_P1,
    SIDEKICK_P2,

    DRACULA_ABILITY,
    DONE,

    MANEUVER_BOOST,
    MANEUVER_SELECT_FIGHTER,
    MANEUVER_SELECT_ZONE,
    DRACULA_CARD_BEAST,

    SHERLOCK_ABILITY,
    DEFENSE_SELECT,
    CARD_SELECT_ZONE,
    INVISIBLE_REAPPEAR,
    POST_COMBAT_ZONE_SELECT,
    POST_COMBAT_HAND_DISPLAY,
    ATTACKER_SELECT,

    // SAVE / LOAD
    SAVE_SLOTS,
    LOAD_SLOTS
};


    Battle battle;                 
    SetupState setupState = SetupState::MAIN_MENU;
    sf::Font font;
    sf::Text promptText;
    std::string ageInput;
    int age1 = 0, age2 = 0;
    sf::Texture sherlocktex, draculatex, watsontex, sistertex[3],invisibletex,fogtex;
    bool sherlocktexOk = false, draculatexOk = false, watsontexOk = false, sistertexOk[3] = {false,false,false},invisibletOk = false,fogOk=false;


    struct HeroButton
    { sf::RectangleShape box; 
        sf::Text label;
        Fighter* hero = nullptr;
        explicit HeroButton(const sf::Font& f):label(f){} 
        };

   struct CardWidget
    {
        sf::RectangleShape box;
        sf::Text nameText;
        sf::Text valueText;
        int handIndex = -1;
        bool usable = false;
        sf::Texture* tex = nullptr;
        explicit CardWidget(const sf::Font& f) : nameText(f), valueText(f) {}
    };
    Card* pendingCard = nullptr;
    Fighter* pendingCardAttacker = nullptr;
    int pendingCardOriginalIndex = -1;
    Fighter* pendingCardDefender = nullptr;
    bool pendingCardIsDefense = false;
    sf::RectangleShape sherlockYesBox;
    sf::Text sherlockYesText;
    bool pendingIsDefenseSelection = false;
    int pendingDefenseCardIndex = -1;
    int pendingDefenseGuiChoice = -1;

    void handleCardZoneClick(sf::Vector2f pos);
    void drawCardSelectedZoneUI();
    std::vector<CardWidget> handCards;
    std::map<Cardname, sf::Texture> cardTextures;
    std::vector<HeroButton> heroButtons;
    std::vector<int> validZoneIds;
    std::vector<Fighter*> abilityTargets;
    sf::RectangleShape abilityCancelBox;
    sf::Text abilityCancelText;
    
    sf::RectangleShape maneuverButton;
    sf::Text maneuverButtonText;

    std::vector<CardWidget> boostCards;
    sf::RectangleShape noBoostBox;
    sf::Text noBoostText;

    std::vector<HeroButton> maneuverFighterButtons;
    sf::RectangleShape maneuverdonebox;
    sf::Text maneuverdonetext;
    int selectedCardIndex = -1;

    Fighter* selectedCardFighter = nullptr;

    std::vector<int> validCardZoneIds;
    std::vector<Fighter*> validCardFighters;


    int maneuverMax = 0;
    std::vector<Fighter*> maneuverMovable;
    std::vector<bool> maneuverMoved;
    Fighter* maneuverSelectedFighter = nullptr;

    sf::RectangleShape cancelDefenseBox;
    sf::Text cancelDefenseText;
    std::vector<CardWidget> defenseCardsUI;

    Fighter* pendingAttacker;
    Fighter* pendingDefender;
    int pendingAttackCardIndex;

    void loadFighterTextures();
    void drawFighters();
    void drawfogtoken();
    void handleAgeTextInput(char32_t );
    void setupHeroButtons();
    void handleHeroClick(sf::Vector2f );
    void startSidekickStage();
    void handleBoardClick(sf::Vector2f );
    void beginTurnFlow();
    void handleAbilityClick(sf::Vector2f pos);
    void drawAbilityUI();
    void startManeuver();
    void setupBoostCards();
    void drawBoostCards();
    void handleBoostClick(sf::Vector2f pos);
    void beginManeuverMovement();
    void setupManeuverFighterButtons();
    void drawManeuverFighterButtons();
    void handleManeuverFighterClick(sf::Vector2f pos);
    void handleManeuverZoneClick(sf::Vector2f pos);
    void finishManeuver();
    void setupHandCards();
    void loadCardTextures();
    void drawHandCards();
    void handleCardClick(sf::Vector2f );
    void handleHandSelectionClick(sf::Vector2f pos);
    void drawHandSelectionUI();
    void finishHandSelection();
    void proceedToAttack(Fighter* attacker, Fighter* defender, Fighter* cardOwner, int cardIndex);
    void populateHandWidgets(std::vector<CardWidget>& widgets, Fighter* fighter, int excludeIndex = -1);
    void setupNumberPicker(int maxN);
    void handleSherlockAbilityClick(sf::Vector2f pos);
    void drawSherlockAbilityUI();

    void drawSetupUI();

void setupDefenseCards(Fighter* defender, const std::vector<int>& defIndexes);
    void handleDefenseClick(sf::Vector2f pos);
    void drawDefenseUI();
    void finishAttack();

    std::vector<HeroButton> mainMenuButtons;
    sf::RectangleShape combatMessageBar;
    sf::Text combatMessageText;



    sf::RectangleShape saveGameBox;
    sf::Text saveGameText;

    // Save / Load slots
    std::vector<HeroButton> saveSlotButtons;
    std::vector<HeroButton> loadSlotButtons;

    void setupMainMenu();
    void handleMainMenuClick(sf::Vector2f pos);
    void drawMainMenu();

    void setupSaveGameButton();
    void handleSaveGameClick(sf::Vector2f pos);
    void drawSaveGameButton();

    void handleInvisibleReappearClick(sf::Vector2f pos);
    void drawCombatMessage();

    // Save / Load slot UI
    void setupSaveSlots();
    void setupLoadSlots();

    void handleSaveSlotClick(sf::Vector2f pos);
    void handleLoadSlotClick(sf::Vector2f pos);

    void drawSaveSlots();
    void drawLoadSlots();




    CardEffect* postCombatEffect = nullptr;
    Fighter* postCombatAttacker = nullptr;
    Fighter* postCombatDefender = nullptr;
    int pendingAttackerCardIndex = -1;

    void playCardWithAttacker(Fighter* attacker, Card& chosenCard, int handIndex);
    void handlePostCombatZoneClick(sf::Vector2f pos);
    void handlePostCombatHandDisplayClick(sf::Vector2f pos);
    void drawPostCombatHandDisplay();
    void handleAttackerSelectClick(sf::Vector2f pos);
    void advanceAfterAction();

    bool helpOpen = false;
    int helpPageIndex = 0;
    std::vector<std::string> helpPages;

    sf::Font helpFont;

    sf::RectangleShape helpButtonBox;
    sf::Text helpButtonText;

    sf::RectangleShape helpOverlayBg;
    sf::RectangleShape helpPanelBox;
    sf::Text helpBodyText;
    sf::Text helpPageIndicatorText;

    sf::RectangleShape helpCloseBox;
    sf::Text helpCloseText;
    sf::RectangleShape helpPrevBox;
    sf::Text helpPrevText;
    sf::RectangleShape helpNextBox;
    sf::Text helpNextText;

    void setupHelpUI();
    void buildHelpPages();
    void handleHelpClick(sf::Vector2f pos);
    void drawHelpButton();
    void drawHelpOverlay();

public:
    GraphicManager();

    void run();
};


#endif