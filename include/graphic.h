#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>
#include "battle.h"

struct Spot
{
    int id;
    sf::CircleShape circle;
};

class GraphicManager
{
private:
    sf::RenderWindow window;
    sf::Texture backgroundTexture;
    std::optional<sf::Sprite> background;
    
    std::vector<Spot> boardSpots;

     std::vector<sf::Texture> spotTextures;
    std::vector<std::pair<int, int>> edges;

     void initEdges(); 
    void initSpots();

    enum class SetupState {AGE_P1, AGE_P2, HERO_1, HERO_2, SIDEKICK_P1, SIDEKICK_P2, DONE };
    Battle battle;                 
    SetupState setupState = SetupState::AGE_P1;
    sf::Font font;
    sf::Text promptText;
    std::string ageInput;
    int age1 = 0, age2 = 0;

    struct HeroButton { sf::RectangleShape box; sf::Text label; Fighter* hero = nullptr;explicit HeroButton(const sf::Font& f):label(f){} };
    std::vector<HeroButton> heroButtons;
    std::vector<int> validZoneIds;

    void handleAgeTextInput(char32_t unicode);
    void setupHeroButtons();
    void handleHeroClick(sf::Vector2f pos);
    void startSidekickStage();
    void handleBoardClick(sf::Vector2f pos);
    void drawSetupUI();

public:
    GraphicManager();

    void run();
};

#endif