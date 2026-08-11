#include "graphic.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>
#include <unordered_set>

GraphicManager::GraphicManager()
    : window(
        sf::VideoMode({ static_cast<unsigned int>(sf::VideoMode::getDesktopMode().size.x * 0.90f), static_cast<unsigned int>(sf::VideoMode::getDesktopMode().size.y * 0.90f)}),
        "Unmatched Game",
          sf::Style::Close | sf::Style::Titlebar
       
      ),
      background(backgroundTexture),
      promptText(font)
{
    if (!backgroundTexture.loadFromFile("background.png"))
    {
        std::cerr << "faild load\n";
    }
    else
    {
        background.emplace(backgroundTexture);
        sf::Vector2u size = backgroundTexture.getSize();
        background->setScale({ 1280.f / static_cast<float>(size.x), 720.f / static_cast<float>(size.y) });
    }

    sf::View gameView(sf::FloatRect({0.f, 0.f}, {1280.f, 720.f}));
    window.setView(gameView);

    initSpots();
    initEdges();

    std::cout << "ok load\n";
}

void GraphicManager::initSpots()
{
    

    sf::Texture tex1, tex2, tex3, tex4,tex5,tex6 ,tex7,tex8,tex9,tex10,tex11,tex12,tex13,tex14,tex15,tex16;
   if (!tex1.loadFromFile("img1.jpg")) {
        std::cerr << "Error: Could not load img1.jpg\n";
    }
    if (!tex2.loadFromFile("img2.jpg")) {
        std::cerr << "Error: Could not load img2.jpg\n";
    }
    if (!tex3.loadFromFile("img3.jpg")) {
        std::cerr << "Error: Could not load img3.jpg\n";
    }
    if (!tex4.loadFromFile("img4.jpg")) {
        std::cerr << "Error: Could not load img4.jpg\n";
    }
    if (!tex5.loadFromFile("img5.jpg")) {
        std::cerr << "Error: Could not load img4.jpg\n";
    }
    if (!tex6.loadFromFile("img6.jpg")) {
        std::cerr << "Error: Could not load img4.jpg\n";
    }
       if (!tex7.loadFromFile("img7.jpg")) {
        std::cerr << "Error: Could not load img7.jpg\n";
    }
     if (!tex8.loadFromFile("img8.jpg")) {
        std::cerr << "Error: Could not load img8.jpg\n";
    }
    if (!tex9.loadFromFile("img9.jpg")) {
        std::cerr << "Error: Could not load img8.jpg\n";
    }
    if (!tex10.loadFromFile("img10.jpg")) {
        std::cerr << "Error: Could not load img8.jpg\n";
    }

     if (!tex11.loadFromFile("img11.jpg")) {
        std::cerr << "Error: Could not load img4.jpg\n";
    }
       if (!tex12.loadFromFile("img12.jpg")) {
        std::cerr << "Error: Could not load img7.jpg\n";
    }
     if (!tex13.loadFromFile("img13.jpg")) {
        std::cerr << "Error: Could not load img8.jpg\n";
    }
    if (!tex14.loadFromFile("img14.jpg")) {
        std::cerr << "Error: Could not load img8.jpg\n";
    }
    if (!tex15.loadFromFile("img15.jpg")) {
        std::cerr << "Error: Could not load img8.jpg\n";
    }
    if (!tex16.loadFromFile("img16.jpg")) {
        std::cerr << "Error: Could not load img8.jpg\n";
    }

    spotTextures = {tex1, tex2, tex3, tex4,tex5,tex6,tex7 ,tex8 ,tex9,tex10,tex11,tex12,tex13,tex14,tex15,tex16};


    float radius = 32.f;

    sf::Vector2f positions[33] = {
        {0, 0},
        {784.444f, 300.741f}, // 1
        { 602.963f, 59.2593f},  // 2
        {933.333f, 260.741f}, // 3
        {640.741f, 282.963f}, // 4
        {574.074f,  207.407f}, // 5
        {477.037f, 258.519f}, // 6
        {549.63f, 305.926f}, // 7
        {397.778f, 223.704f}, // 8
        {287.407f,351.852f}, // 9
        {233.333f,  252.593f}, // 10
        {249.63f, 162.963f}, // 11
        {462.963f, 167.407f}, // 12
        {289.63f, 80.7407f}, // 13
        {380.f, 53.3333f}, // 14
        {431.111f, 99.2593f}, // 15
        {530.37f, 70.3704f},  // 16
        {892.593f, 328.148f}, // 17
        {670.37f, 54.8148f}, // 18
        {665.185f,  151.111f}, // 19
        {757.037f, 80.f}, // 20
        {804.444f, 148.148f}, // 21
        {939.259f, 128.889f},// 22
        {864.444f,  39.2592f},// 23
        {829.63f, 225.185f},// 24
        {848.889f, 408.889f}, // 25
        {822.963f, 468.889f}, // 26
        {725.185f, 369.63f}, // 27
        {659.259f, 397.037f}, // 28
        {591.111f, 427.407f}, // 29
        {518.519f, 451.852f}, // 30
        {428.889f,  405.185f}, // 31
        {340.741f, 397.778f}  // 32
    };

    std::unordered_set<int>pinkspots={1,3,17,4};
    std::unordered_set<int>grayspots={29,28,27,26 , 30 , 31 ,32};
    std::unordered_set<int>bluespots={13 , 14 , 15 ,16};
    std::unordered_set<int>brownspots={18 , 19 , 20 ,16};
    std::unordered_set<int>yellowspots={22 , 24 , 23 };
    std::unordered_set<int>greenspots={ 5 , 6 };
    std::unordered_set<int>darkbulespots={ 10 , 11 };
    std::unordered_set<int>darkbuleandbulespots={ 12 };
    std::unordered_set<int>DBgreenspots={ 8 };
    std::unordered_set<int>pinkgreenspots={4};
    std::unordered_set<int>pinkgrayspots={ 25};
    std::unordered_set<int>greengrayspots={ 7 };
    std::unordered_set<int>bluebrownspots={2 };
    std::unordered_set<int>brownyellowgreenspots={ 21};
    std::unordered_set<int>dbgrayspots={ 9};



    for (int i = 1; i <= 32; ++i)
    {
        Spot spot;
        spot.id = i;
        spot.circle.setRadius(radius);
        spot.circle.setOrigin({radius, radius});
        spot.circle.setPosition(positions[i]);
        
        if (pinkspots.count(i) ) 
        {
            spot.circle.setTexture(&spotTextures[0]); 
        }
        else if (grayspots.count(i)) 
        {
            spot.circle.setTexture(&spotTextures[1]); 
        }
        else if (greenspots.count(i)) 
        {
            spot.circle.setTexture(&spotTextures[2]); 
        }
        else if (bluespots.count(i)) 
        {
            spot.circle.setTexture(&spotTextures[3]); 
        }
        else if(yellowspots.count(i))
        {
           spot.circle.setTexture(&spotTextures[4]); 
        }
        else if(brownspots.count(i))
        {
           spot.circle.setTexture(&spotTextures[5]); 
        }
          else if(darkbulespots.count(i))
        {
           spot.circle.setTexture(&spotTextures[6]); 
        }
         else if(darkbuleandbulespots.count(i))
        {
           spot.circle.setTexture(&spotTextures[7]); 
        }
             else if(DBgreenspots.count(i))
        {
           spot.circle.setTexture(&spotTextures[8]); 
        }
        else if(pinkgrayspots.count(i))
        {
           spot.circle.setTexture(&spotTextures[10]); 
        }
         else if(pinkgrayspots.count(i))
        {
           spot.circle.setTexture(&spotTextures[11]); 
        }
          else if(bluebrownspots.count(i))
        {
           spot.circle.setTexture(&spotTextures[12]); 
        }
         else if(brownyellowgreenspots.count(i))
        {
           spot.circle.setTexture(&spotTextures[13]); 
        }
         else if(dbgrayspots.count(i))
        {
           spot.circle.setTexture(&spotTextures[14]); 
        }

          else if(greengrayspots.count(i))
        {
           spot.circle.setTexture(&spotTextures[15]); 
        }
        
        

         spot.circle.setOutlineThickness(-3.f); 

    spot.circle.setOutlineColor(sf::Color(70, 55, 40)); 

        boardSpots.push_back(spot);
    }
}

void GraphicManager::initEdges()
{
    edges = {
        {1, 17}, {3, 17}, {17, 25}, {25, 26}, {25, 27}, {26, 27},
        {27, 28}, {28, 4}, {28, 29}, {29, 30}, {29, 7}, {30, 31},
        {30, 7}, {31, 32}, {32, 9}, {9, 10}, {10, 11}, {11, 12},
        {11, 13}, {13, 14}, {14, 15}, {15, 16}, {16, 2}, {2, 12},
        {2, 18}, {18, 19}, {18, 20}, {19, 20}, {19, 21}, {20, 21},
        {21, 22}, {21, 24}, {21, 4}, {22, 23}, {22, 24}, {24, 3},
        {4, 1}, {4, 5}, {5, 6}, {6, 7}, {6, 8}, {7, 28}, {8, 9}, {4, 27}
    };
}

void GraphicManager::run()
{
  
    if (!font.openFromFile("player.ttf"))   
    std::cerr << "font load failed\n";

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            if (setupState == SetupState::AGE_P1 || setupState == SetupState::AGE_P2) {
    if (const auto* txt = event->getIf<sf::Event::TextEntered>())
        handleAgeTextInput(txt->unicode);
}
else if (setupState == SetupState::HERO_1 || setupState == SetupState::HERO_2) {
    if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
        if (mb->button == sf::Mouse::Button::Left)
            handleHeroClick(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
    }
}

        else if(setupState == SetupState::DONE)
        {
            if (const auto* mouseBtn = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouseBtn->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    
                    std::cout << "Clicked -> X: " << mousePos.x << " | Y: " << mousePos.y << std::endl;

                    bool hitSpot = false;for (const auto& spot : boardSpots)
                    {
                        sf::Vector2f center = spot.circle.getPosition();
                        float dx = mousePos.x - center.x;
                        float dy = mousePos.y - center.y;
                        
                        if (std::sqrt(dx * dx + dy * dy) <= spot.circle.getRadius())
                        {
                            std::cout << "-> Clicked on Spot #" << spot.id << std::endl;
                            hitSpot = true;
                            break;
                        }
                    }
                }
            }
        }
        }

      window.clear();

        if (background)
        {
            window.draw(*background);
        }

        if (setupState == SetupState::AGE_P1 || setupState == SetupState::AGE_P2 ||
            setupState == SetupState::HERO_1 || setupState == SetupState::HERO_2)
        {
            drawSetupUI();
        }
        else
        {
            for (const auto& edge : edges)
            {
                sf::Vector2f p1 = boardSpots[edge.first - 1].circle.getPosition();
                sf::Vector2f p2 = boardSpots[edge.second - 1].circle.getPosition();
                sf::VertexArray line(sf::PrimitiveType::Lines, 2);
                line[0].position = p1; line[0].color = sf::Color(250, 200, 200, 150);
                line[1].position = p2; line[1].color = sf::Color(250, 200, 200, 150);
                window.draw(line);
            }

            for (const auto& spot : boardSpots)
            {
                bool highlight = false;
                if (setupState == SetupState::SIDEKICK_P1 || setupState == SetupState::SIDEKICK_P2) {
                    for (int id : validZoneIds) if (id == spot.id) { highlight = true; break; }
                }
                if (highlight) {
                    sf::CircleShape glow = spot.circle;
                    glow.setOutlineThickness(-5.f);
                    glow.setOutlineColor(sf::Color::Yellow);
                    window.draw(glow);
                } else {
                    window.draw(spot.circle);
                }
            }
        }

        window.display();
    }

}

void GraphicManager::handleAgeTextInput(char32_t u) {
    if (u == 8) { if (!ageInput.empty()) ageInput.pop_back(); return; }      // Backspace
    if (u == 13) {                                                          // Enter
        if (ageInput.empty()) return;
        int age = std::stoi(ageInput);
        if (age < 1 || age > 100) { ageInput.clear(); return; }
        if (setupState == SetupState::AGE_P1) {
            age1 = age; ageInput.clear();
            setupState = SetupState::AGE_P2;
        } else {
            age2 = age; ageInput.clear();
            battle.setAges(age1, age2);
            setupState = SetupState::HERO_1;
            setupHeroButtons();
        }
        return;
    }
    if (u >= '0' && u <= '9' && ageInput.size() < 3) ageInput += static_cast<char>(u);
}

void GraphicManager::setupHeroButtons() {
    heroButtons.clear();
    float y = 200.f;
    for (Fighter* h : battle.getAvailableHeroes()) {
        HeroButton b(font);
        b.box.setSize({250.f, 60.f});
        b.box.setPosition({100.f, y});
        b.box.setFillColor(sf::Color(123,63,0));
        b.box.setOutlineThickness(3.f);
        b.box.setOutlineColor(sf::Color::White);
        b.label.setString(h->getName());
        b.label.setCharacterSize(24);
        b.label.setPosition({120.f, y + 15.f});
        b.hero = h;
        heroButtons.push_back(b);
        y += 90.f;
    }
}

void GraphicManager::handleHeroClick(sf::Vector2f pos)
{
    for (auto& b : heroButtons) {
        if (b.box.getGlobalBounds().contains(pos)) {
            Player& chooser = (setupState == SetupState::HERO_1)
                               ? battle.getFirstChooser() : battle.getSecondChooser();
            battle.assignHero(chooser, b.hero);
            if (setupState == SetupState::HERO_1) {
                setupState = SetupState::HERO_2;
                setupHeroButtons();
            } else {
                battle.beginUnitSetup();
                setupState = SetupState::SIDEKICK_P1;
                startSidekickStage();
            }
            return;
        }
    }
}

void GraphicManager::startSidekickStage() {
    Player& current = (setupState == SetupState::SIDEKICK_P1)
                       ? battle.getplayer1() : battle.getplayer2();
    validZoneIds = battle.getSidekickValidZones(current);

    if (validZoneIds.empty()) {
        if (setupState == SetupState::SIDEKICK_P1) {
            setupState = SetupState::SIDEKICK_P2;
            startSidekickStage();
        } else {
            battle.finalizeSetup();
            setupState = SetupState::DONE;
        }
    }
}

void GraphicManager::handleBoardClick(sf::Vector2f pos) 
{
    for (const auto& spot : boardSpots) {
        sf::Vector2f center = spot.circle.getPosition();
        float dx = pos.x - center.x, dy = pos.y - center.y;
        if (std::sqrt(dx * dx + dy * dy) > spot.circle.getRadius()) continue;

        bool isValid = false;
        for (int id : validZoneIds) if (id == spot.id) { isValid = true; break; }
        if (!isValid) return;

        Player& current = (setupState == SetupState::SIDEKICK_P1)
                           ? battle.getplayer1() : battle.getplayer2();
        bool stepDone = battle.placeSidekickAt(current, spot.id);

        if (stepDone) {
            if (setupState == SetupState::SIDEKICK_P1) {
                setupState = SetupState::SIDEKICK_P2;
                startSidekickStage();
            } else {
                battle.finalizeSetup();
                setupState = SetupState::DONE;
            }
        } else {
            validZoneIds = battle.getSidekickValidZones(current); 
        }
        return;
    }
}

void GraphicManager::drawSetupUI()
 {
    std::string msg;
    if (setupState == SetupState::AGE_P1) msg = "Player  (1)  age: " + ageInput;
    else if (setupState == SetupState::AGE_P2) msg = "Player (2)    age: " + ageInput;
    else msg = "Choose   your  hero";
    promptText.setFont(font);
    promptText.setString(msg);
    promptText.setCharacterSize(28);
    promptText.setPosition({50.f, 50.f});
    window.draw(promptText);
    for (auto& b : heroButtons) { window.draw(b.box); window.draw(b.label); }
}