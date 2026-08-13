#include "graphic.h"
#include "boardmanager.h"
#include "combatmanager.h"
#include "cardeffect.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>
#include <unordered_set>

static std::string sanitizeCardFilename(const std::string& title)
{
    std::string s;
    for (char c : title) {
        if (std::isalnum(static_cast<unsigned char>(c)))
            s += std::tolower(static_cast<unsigned char>(c));
    }
    return s + ".jpg";
}

GraphicManager::GraphicManager() : window(sf::VideoMode({ static_cast<unsigned int>(sf::VideoMode::getDesktopMode().size.x * 0.90f), static_cast<unsigned int>(sf::VideoMode::getDesktopMode().size.y * 0.90f)}),
        "Unmatched Game", sf::Style::Close | sf::Style::Titlebar ),
     promptText(font), abilityCancelText(font), maneuverButtonText(font), noBoostText(font),maneuverdonetext(font),cancelDefenseText(font)
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

        cancelDefenseBox.setSize({60.f, 60.f});
        cancelDefenseBox.setPosition({20.f, 630.f}); // پایین سمت چپ
        cancelDefenseBox.setFillColor(sf::Color(180, 40, 40));
        cancelDefenseBox.setOutlineThickness(2.f);
        cancelDefenseBox.setOutlineColor(sf::Color::White);

        cancelDefenseText.setString("X");
        cancelDefenseText.setCharacterSize(30);
        cancelDefenseText.setFillColor(sf::Color::White);
        cancelDefenseText.setPosition({40.f, 640.f});

        sf::View gameView(sf::FloatRect({0.f, 0.f}, {1280.f, 720.f}));
        window.setView(gameView);

        initSpots();
        initEdges();
        loadFighterTextures();
        loadCardTextures();
        abilityCancelBox.setSize({60.f, 60.f});
        abilityCancelBox.setPosition({20.f, 630.f});
        abilityCancelBox.setFillColor(sf::Color(180, 40, 40));
        abilityCancelBox.setOutlineThickness(2.f);
        abilityCancelBox.setOutlineColor(sf::Color::White);

        abilityCancelText.setString("X");
        abilityCancelText.setCharacterSize(30);
        abilityCancelText.setFillColor(sf::Color::White);
        abilityCancelText.setPosition({40.f, 640.f});
        maneuverButton.setSize({140.f, 60.f});
        maneuverButton.setPosition({20.f, 630.f});
        maneuverButton.setFillColor(sf::Color(70, 70, 140));
        maneuverButton.setOutlineThickness(2.f);
        maneuverButton.setOutlineColor(sf::Color::White);
        maneuverButtonText.setString("Maneuver");
        maneuverButtonText.setCharacterSize(18);
        maneuverButtonText.setFillColor(sf::Color::White);
        maneuverButtonText.setPosition({30.f, 650.f});

        noBoostBox.setSize({140.f, 60.f});
        noBoostBox.setPosition({20.f, 630.f});
        noBoostBox.setFillColor(sf::Color(120, 40, 40));
        noBoostBox.setOutlineThickness(2.f);
        noBoostBox.setOutlineColor(sf::Color::White);
        noBoostText.setString("No Boost");
        noBoostText.setCharacterSize(16);
        noBoostText.setFillColor(sf::Color::White);
        noBoostText.setPosition({30.f, 650.f});

        maneuverdonebox.setSize({140.f, 60.f});
        maneuverdonebox.setPosition({20.f, 630.f});
        maneuverdonebox.setFillColor(sf::Color(40, 120, 40));
        maneuverdonebox.setOutlineThickness(2.f);
        maneuverdonebox.setOutlineColor(sf::Color::White);
        maneuverdonetext.setString("Done");
        maneuverdonetext.setCharacterSize(18);
        maneuverdonetext.setFillColor(sf::Color::White);
        maneuverdonetext.setPosition({60.f, 650.f});

        

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
        {764.444f, 445.185f}, // 26
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
    void GraphicManager::loadFighterTextures()
    {
        sherlocktexOk = sherlocktex.loadFromFile("sherlock.jpg");
        draculatexOk  = draculatex.loadFromFile("dracula.jpg");
        watsontexOk   = watsontex.loadFromFile("watson.jpg");
        sistertexOk[0]   = sistertex[0].loadFromFile("sister1.jpg");
        sistertexOk[1]   = sistertex[1].loadFromFile("sister2.jpg");
        sistertexOk[02]   = sistertex[2].loadFromFile("sister3.jpg");
        invisibletOk  = invisibletex.loadFromFile("invisibleman.jpg");
        fogOk  = fogtex.loadFromFile("fogtoken.jpg");


        if (!sherlocktexOk) std::cerr << "Error: Could not load sherlock.jpg\n";
        if (!draculatexOk)  std::cerr << "Error: Could not load dracula.jpg\n";
        if (!watsontexOk)   std::cerr << "Error: Could not load watson.jpg\n";
        if (!invisibletOk)   std::cerr << "Error: Could not load watson.jpg\n";
        if (!fogOk)   std::cerr << "Error: Could not load watson.jpg\n";
        if (!sistertexOk[0])   std::cerr << "Error: Could not load sister1.jpg\n";
        if (!sistertexOk[1])   std::cerr << "Error: Could not load sister2.jpg\n";
        if (!sistertexOk[2])   std::cerr << "Error: Could not load sister3.jpg\n";
    }

    void GraphicManager::loadCardTextures()
        {
            auto loadDB = [&](const std::vector<Cardinfo>& db) {
                for (const Cardinfo& info : db) {
                    std::string filename = sanitizeCardFilename(info.title);
                    sf::Texture tex;
                    bool ok = tex.loadFromFile(filename);
                    if (!ok) {
                        std::cerr << "Error: Could not load card image \"" << filename
                                << "\" for card \"" << info.title << "\"\n";
                    }
                    cardTextures[info.name] = std::move(tex);
                }
            };

            loadDB(Card::draculaCardDB);
            loadDB(Card::sherlockCardDB);
            loadDB(Card::invisiblemanCardDB);
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
            if (setupState == SetupState::AGE_P1 || setupState == SetupState::AGE_P2) 
            {
                if (const auto* txt = event->getIf<sf::Event::TextEntered>())
                handleAgeTextInput(txt->unicode);
            }
            else if (setupState == SetupState::HERO_1 || setupState == SetupState::HERO_2) {
                if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mb->button == sf::Mouse::Button::Left)
                        handleHeroClick(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
                }
            }
            else if (setupState == SetupState::SIDEKICK_P1 || setupState == SetupState::SIDEKICK_P2) {
                if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mb->button == sf::Mouse::Button::Left)
                        handleBoardClick(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
                }
            }
            else if (setupState == SetupState::DRACULA_ABILITY) {
                if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mb->button == sf::Mouse::Button::Left)
                        handleAbilityClick(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
                }
            }
            else if (setupState == SetupState::MANEUVER_BOOST) {
                if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mb->button == sf::Mouse::Button::Left)
                        handleBoostClick(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
                }
            }
            else if (setupState == SetupState::MANEUVER_SELECT_FIGHTER) {
                if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mb->button == sf::Mouse::Button::Left)
                        handleManeuverFighterClick(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
                }
            }
            else if (setupState == SetupState::MANEUVER_SELECT_ZONE) {
                if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mb->button == sf::Mouse::Button::Left)
                        handleManeuverZoneClick(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
                }
            }
            else if (setupState == SetupState::DEFENSE_SELECT) {
    if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
        if (mb->button == sf::Mouse::Button::Left)
            handleDefenseClick(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
    }
}

else if (setupState == SetupState::CARD_SELECT_ZONE) {
    if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
        if (mb->button == sf::Mouse::Button::Left) {
            handleCardZoneClick(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
        }
    }
}
            else if(setupState == SetupState::DONE)
            {
                if (const auto* mouseBtn = event->getIf<sf::Event::MouseButtonPressed>())
                {
                    if (mouseBtn->button == sf::Mouse::Button::Left)
                    {
                        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    handleCardClick(mousePos);
                        if (maneuverButton.getGlobalBounds().contains(mousePos))
                        {
                            startManeuver();
                        }
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
            else if (setupState == SetupState::DEFENSE_SELECT) {
                    drawDefenseUI();
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
                for (const auto& spot : boardSpots)
                {
                      bool highlight = false;
                        if (setupState == SetupState::SIDEKICK_P1 || setupState == SetupState::SIDEKICK_P2 || 
                            setupState == SetupState::MANEUVER_SELECT_ZONE || setupState == SetupState::CARD_SELECT_ZONE) 
                        {
                            for (int id : validZoneIds) 
                                if (id == spot.id) { highlight = true; break; }
                        }

                    if (highlight) 
                    {
                         sf::CircleShape glow = spot.circle;
                         glow.setOutlineThickness(-5.f);
                         glow.setOutlineColor(sf::Color::Yellow);
                         window.draw(glow);
                    }
                    else 
                    {
                        window.draw(spot.circle);
                    }
                }
                

                drawFighters(); 
                drawfogtoken();

                if (setupState == SetupState::DONE) 
                {
                    drawHandCards();
                    window.draw(maneuverButton);
                    window.draw(maneuverButtonText);
                } 
                
                else if (setupState == SetupState::DRACULA_ABILITY)
                {
                    drawAbilityUI();
                } 
                else if (setupState == SetupState::MANEUVER_BOOST) 
                {
                    drawBoostCards();
                } 
                else if (setupState == SetupState::MANEUVER_SELECT_FIGHTER) 
                {
                    drawManeuverFighterButtons();
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

    void GraphicManager::drawFighters()
    {
        auto zonePos = [&](int zoneId) -> sf::Vector2f {
            for (const auto& spot : boardSpots)
                if (spot.id == zoneId) return spot.circle.getPosition();
            return {0.f, 0.f};
        };

        auto drawAt = [&](Fighter& f, sf::Texture& tex, bool texOk, sf::Color fallback)
        {
            if (!f.isalive() || f.getPosition() == nullptr) return;
            sf::Vector2f pos = zonePos(f.getPosition()->getId());

            float radius = 32.f;
            sf::CircleShape marker(radius);
            marker.setOrigin({radius, radius});
            marker.setPosition(pos);
            marker.setOutlineThickness(-3.f);
            marker.setOutlineColor(sf::Color::Black);

            if (texOk) {
                marker.setTexture(&tex);
                marker.setFillColor(sf::Color(175,175,175));
            } else {
                marker.setFillColor(fallback);
            }

            window.draw(marker);
        };

        drawAt(battle.getSherlock(),     sherlocktex,  sherlocktexOk,  sf::Color::Blue);
        drawAt(battle.getWatson(),       watsontex,    watsontexOk,    sf::Color(100, 100, 255));
        drawAt(battle.getDracual(),      draculatex,   draculatexOk,   sf::Color::Red);
        drawAt(battle.getInvisibleMan(), invisibletex, invisibletOk, sf::Color(150, 150, 150));

        vector<Sisters>& sisters = battle.getsisters();
        for (int i = 0; i < (int)sisters.size() && i < 3; i++) {
            drawAt(sisters[i], sistertex[i], sistertexOk[i], sf::Color(255, 100, 100));
        }
    }

    void GraphicManager::drawfogtoken()
    {
        auto zonePos = [&](int zoneId) -> sf::Vector2f {
            for (const auto& spot : boardSpots)
                if (spot.id == zoneId) return spot.circle.getPosition();
            return {0.f, 0.f};
        };

        for (FogToken& fog : battle.getfogtoken()) {
            if (fog.getPosition() == nullptr) continue;
            sf::Vector2f pos = zonePos(fog.getPosition()->getId());

            float radius = 32.f;
            sf::CircleShape marker(radius);
            marker.setOrigin({radius, radius});
            marker.setPosition(pos);
            marker.setOutlineThickness(-3.f);
            marker.setOutlineColor(sf::Color(80, 80, 80));

            if (fogOk) {
                marker.setTexture(&fogtex);
            } else {
                marker.setFillColor(sf::Color(200, 200, 200, 150));
            }

            window.draw(marker);
        }
    }

        void GraphicManager::startSidekickStage() {
            Player& current = (setupState == SetupState::SIDEKICK_P1)
                            ? battle.getplayer1() : battle.getplayer2();
            validZoneIds = battle.getSidekickValidZones(current);

            if (validZoneIds.empty()) 
            {
                if (setupState == SetupState::SIDEKICK_P1) 
                {
                    setupState = SetupState::SIDEKICK_P2;
                    startSidekickStage();
                }
                else
                {
                   battle.finalizeSetup();
                    battle.startGame();
                    beginTurnFlow();
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
    
                        battle.startGame();
                        beginTurnFlow();
                       
                    }
                } else {
                    validZoneIds = battle.getSidekickValidZones(current); 
                }
                return;
            }
        }

         void GraphicManager::setupHandCards()
        {
            handCards.clear();

            Player& current = battle.getCurrentPlayer();
            Player& other = battle.getOtherPlayer(current);
            vector<Card>& hand = current.getHero()->gethand();
            vector<AttackCardInfo> info = current.getHero()->getPlayableCardIndexes(&battle, other.getHero(), current.getHero());

            float cardW = 120.f, cardH = 160.f, gap = 20.f;
            float totalW = info.size() * cardW + (info.empty() ? 0 : (info.size() - 1) * gap);
            float startX = (1280.f - totalW) / 2.f;
            float y = 720.f - cardH - 20.f;

            for (size_t i = 0; i < info.size(); i++) 
            {
                CardWidget cw(font);
                float x = startX + i * (cardW + gap);

                cw.box.setSize({cardW, cardH});
                cw.box.setPosition({x, y});
                cw.box.setOutlineThickness(2.f);
                cw.box.setOutlineColor(sf::Color::Black);
                cw.usable = info[i].usable;
                cw.handIndex = info[i].index;

                Card& c = hand[info[i].index];

                auto it = cardTextures.find(c.getcardname());
                if (it != cardTextures.end()) 
                {
                    cw.tex = &it->second;
                    cw.box.setTexture(cw.tex);
                    cw.box.setFillColor(cw.usable ? sf::Color::White : sf::Color(120, 120, 120));
                } 
                else 
                {
                    cw.box.setFillColor(cw.usable ? sf::Color(230, 220, 190) : sf::Color(120, 120, 120));
                    cw.nameText.setString(c.getName());
                    cw.nameText.setCharacterSize(16);
                    cw.nameText.setFillColor(sf::Color::Black);
                    cw.nameText.setPosition({x + 8.f, y + 8.f});
                }

                cw.valueText.setString("V:" + std::to_string(c.getValue()) + " B:" + std::to_string(c.getBoost()));
                cw.valueText.setCharacterSize(14);
                cw.valueText.setFillColor(cw.tex ? sf::Color::Yellow : sf::Color::Black);
                cw.valueText.setPosition({x + 6.f, y + cardH - 26.f});

                handCards.push_back(std::move(cw));
            }
        }
        void GraphicManager::beginTurnFlow()
        {
            Fighter* hero = battle.getCurrentPlayer().getHero();

            if (hero != nullptr && hero->getName() == "Dracula" && hero->isalive()) 
            {
                abilityTargets = battle.getDraculaObj().getAbilityTargets(&battle);
                setupState = SetupState::DRACULA_ABILITY;
            } 
            else 
            {
                setupState = SetupState::DONE;
                setupHandCards();
            }
        }
    void GraphicManager::drawHandCards()
    {
        for (auto& cw : handCards) {
            window.draw(cw.box);
            window.draw(cw.nameText);
            window.draw(cw.valueText);
        }
    }

   void GraphicManager::handleCardClick(sf::Vector2f pos) {
    for (auto& cw : handCards) {
        if (!cw.box.getGlobalBounds().contains(pos)) continue;
        if (!cw.usable) return;
        Player& current = battle.getCurrentPlayer();
        Player& other = battle.getOtherPlayer(current);
        Card& chosenCard = current.getHero()->gethand()[cw.handIndex];
        Fighter* attacker = nullptr;
        
        if (!current.chooseAttackerIfNeeded(battle, chosenCard, attacker, other.getHero()) || attacker == nullptr) {
            setupHandCards();
            return;
        }

        if (chosenCard.getEffect() != nullptr && chosenCard.getEffect()->needsGUIInput()) {
            pendingCard = &chosenCard;
            pendingCardAttacker = attacker;
            validZoneIds = chosenCard.getEffect()->getValidZones(attacker, &battle);
            setupState = SetupState::CARD_SELECT_ZONE;
            return;
        }

        if (chosenCard.getcardType() == SCHEME) {
            current.playScheme(other, battle, attacker, cw.handIndex); 
            battle.addAction();
        } else {
            pendingAttacker = attacker;
            pendingDefender = other.getHero();
            pendingAttackCardIndex = cw.handIndex;
            
            std::vector<int> defIndexes = battle.getCombat()->getValidDefenseCards(pendingDefender);
            if (defIndexes.empty()) {
                battle.getCombat()->resolveCombat(pendingAttacker, pendingDefender, current.getHero(), pendingAttackCardIndex, -1);
                finishAttack();
                return;
            } else {
                setupDefenseCards(pendingDefender, defIndexes);
                setupState = SetupState::DEFENSE_SELECT;
                return;
            }
        }

        if (battle.turnShouldEnd()) {
            battle.endTurnAndAdvance();
            beginTurnFlow();
        } else {
            setupHandCards();
        }
        return;
    }
}
    void GraphicManager::drawSetupUI()
    {
        std::string msg;
        if (setupState == SetupState::AGE_P1) msg = "Player (1)  age: " + ageInput;
        else if (setupState == SetupState::AGE_P2) msg = "Player (2)    age: " + ageInput;
        else msg = "Choose   your  hero";
        promptText.setFont(font);
        promptText.setString(msg);
        promptText.setCharacterSize(28);
        promptText.setPosition({50.f, 50.f});
        window.draw(promptText);
        for (auto& b : heroButtons) { window.draw(b.box); window.draw(b.label); }
    }
    void GraphicManager::handleAbilityClick(sf::Vector2f pos)
    {
        if (abilityCancelBox.getGlobalBounds().contains(pos)) {
            setupState = SetupState::DONE;
            setupHandCards();
            return;
        }

        for (const auto& spot : boardSpots) {
            sf::Vector2f center = spot.circle.getPosition();
            float dx = pos.x - center.x, dy = pos.y - center.y;
            if (std::sqrt(dx * dx + dy * dy) > spot.circle.getRadius()) continue;

            for (Fighter* target : abilityTargets) {
            if (target->getPosition() != nullptr && target->getPosition()->getId() == spot.id) {
                battle.getDraculaObj().useAbilityOn(target);
                setupState = SetupState::DONE;
                setupHandCards();
                return;
            }
           }
            return;
        }
    }

    void GraphicManager::drawAbilityUI()
    {
        for (Fighter* target : abilityTargets) 
        {
            if (target->getPosition() == nullptr) continue;
            for (const auto& spot : boardSpots) 
            {
                if (spot.id == target->getPosition()->getId()) 
                {
                    sf::CircleShape ring = spot.circle;
                    ring.setFillColor(sf::Color::Transparent);
                    ring.setTexture(nullptr);
                    ring.setOutlineThickness(-5.f);
                    ring.setOutlineColor(sf::Color::Red);
                    window.draw(ring);
                    break;
                }
            }
        }
        window.draw(abilityCancelBox);
        window.draw(abilityCancelText);
    }

    void GraphicManager::startManeuver()
{
    Player& current = battle.getCurrentPlayer();
    current.getHero()->drawBoostMovement();
    setupBoostCards();
    setupState = SetupState::MANEUVER_BOOST;
}

void GraphicManager::setupBoostCards()
{
    boostCards.clear();
    Player& current = battle.getCurrentPlayer();
    vector<Card>& hand = current.getHero()->gethand();

    float cardW = 120.f, cardH = 160.f, gap = 20.f;
    float totalW = hand.size() * cardW + (hand.empty() ? 0 : (hand.size() - 1) * gap);
    float startX = (1280.f - totalW) / 2.f;
    float y = 720.f - cardH - 20.f;

    for (size_t i = 0; i < hand.size(); i++) {
        CardWidget cw(font);
        float x = startX + i * (cardW + gap);

        cw.box.setSize({cardW, cardH});
        cw.box.setPosition({x, y});
        cw.box.setOutlineThickness(2.f);
        cw.box.setOutlineColor(sf::Color::Black);
        cw.usable = true;
        cw.handIndex = (int)i;

        Card& c = hand[i];
        auto it = cardTextures.find(c.getcardname());
        if (it != cardTextures.end()) {
            cw.tex = &it->second;
            cw.box.setTexture(cw.tex);
            cw.box.setFillColor(sf::Color::White);
        } else {
            cw.box.setFillColor(sf::Color(230, 220, 190));
            cw.nameText.setString(c.getName());
            cw.nameText.setCharacterSize(16);
            cw.nameText.setFillColor(sf::Color::Black);
            cw.nameText.setPosition({x + 8.f, y + 8.f});
        }

        cw.valueText.setString("Boost:" + std::to_string(c.getBoost()));
        cw.valueText.setCharacterSize(14);
        cw.valueText.setFillColor(cw.tex ? sf::Color::Yellow : sf::Color::Black);
        cw.valueText.setPosition({x + 6.f, y + cardH - 26.f});

        boostCards.push_back(std::move(cw));
    }
}

void GraphicManager::drawBoostCards()
{
    for (auto& cw : boostCards) {
        window.draw(cw.box);
        window.draw(cw.nameText);
        window.draw(cw.valueText);
    }
    window.draw(noBoostBox);
    window.draw(noBoostText);
}

void GraphicManager::handleBoostClick(sf::Vector2f pos)
{
    Player& current = battle.getCurrentPlayer();
    Fighter* hero = current.getHero();

    if (noBoostBox.getGlobalBounds().contains(pos)) {
        maneuverMax = hero->getmovement();
        beginManeuverMovement();
        return;
    }

    for (auto& cw : boostCards) {
        if (cw.box.getGlobalBounds().contains(pos)) {
            vector<Card>& hand = hero->gethand();
            int boost = hand[cw.handIndex].getBoost();
            hand.erase(hand.begin() + cw.handIndex);
            maneuverMax = hero->getmovement() + boost;
            beginManeuverMovement();
            return;
        }
    }
}

void GraphicManager::beginManeuverMovement()
{
    maneuverMovable.clear();
    Fighter* hero = battle.getCurrentPlayer().getHero();

    for (Fighter* f : battle.getFighters()) {
        if (f != nullptr && f->isalive() && f->getteam() == hero->getteam())
            maneuverMovable.push_back(f);
    }
    maneuverMoved.assign(maneuverMovable.size(), false);

    setupState = SetupState::MANEUVER_SELECT_FIGHTER;
    setupManeuverFighterButtons();
}

void GraphicManager::setupManeuverFighterButtons()
{
    maneuverFighterButtons.clear();
    
    float y = 350.f; 
    float boxW = 160.f, boxH = 40.f, gap = 10.f;
    float x = 20.f;
    float bottomY = 630.f - gap;

    for (size_t i = 0; i < maneuverMovable.size(); i++) 
    {
        if (maneuverMoved[i]) continue;

        HeroButton b(font);
        b.box.setSize({boxW, boxH});
        b.box.setPosition({x, y});
        
        b.box.setFillColor(sf::Color(100, 50, 10 , 150)); 
        b.box.setOutlineThickness(2.f);
        b.box.setOutlineColor(sf::Color::Black);
        
        b.label.setString(maneuverMovable[i]->getName());
        b.label.setCharacterSize(16);
        
        sf::FloatRect textRect = b.label.getLocalBounds();
        
        b.label.setOrigin({ textRect.position.x + textRect.size.x / 2.0f,  textRect.position.y + textRect.size.y / 2.0f });
        
        b.label.setPosition({x + boxW / 2.0f, y + boxH / 2.0f});
        
        b.hero = maneuverMovable[i];
        maneuverFighterButtons.push_back(b);
        
        y += boxH + gap;
    }
}

void GraphicManager::drawManeuverFighterButtons()
{
    for (auto& b : maneuverFighterButtons) 
    {
        window.draw(b.box);
        window.draw(b.label);
    }
    window.draw(maneuverdonebox);
    window.draw(maneuverdonetext);
}

void GraphicManager::handleManeuverFighterClick(sf::Vector2f pos)
{
    if (maneuverdonebox.getGlobalBounds().contains(pos)) 
    {
        finishManeuver();
        return;
    }

    for (auto& b : maneuverFighterButtons) 
    {
        if (b.box.getGlobalBounds().contains(pos))
        {
            maneuverSelectedFighter = b.hero;
            validZoneIds.clear();
            vector<Zone*> zones = battle.getBoard()->getReachableZone(*maneuverSelectedFighter, maneuverMax);
            for (Zone* z : zones) validZoneIds.push_back(z->getId());
            setupState = SetupState::MANEUVER_SELECT_ZONE;
            return;
        }
    }
}

void GraphicManager::handleManeuverZoneClick(sf::Vector2f pos)
{
    for (const auto& spot : boardSpots) {
        sf::Vector2f center = spot.circle.getPosition();
        float dx = pos.x - center.x, dy = pos.y - center.y;
        if (std::sqrt(dx * dx + dy * dy) > spot.circle.getRadius()) continue;

        bool valid = false;
        for (int id : validZoneIds) if (id == spot.id) { valid = true; break; }
        if (!valid) return;

        if (battle.getBoard()->movefighter(*maneuverSelectedFighter, spot.id, maneuverMax)) {
            for (size_t i = 0; i < maneuverMovable.size(); i++)
                if (maneuverMovable[i] == maneuverSelectedFighter) maneuverMoved[i] = true;
        }

        maneuverSelectedFighter = nullptr;
        validZoneIds.clear();

        bool anyLeft = false;
        for (bool m : maneuverMoved) if (!m) { anyLeft = true; break; }

        if (!anyLeft) {
            finishManeuver();
        } else {
            setupState = SetupState::MANEUVER_SELECT_FIGHTER;
            setupManeuverFighterButtons();
        }
        return;
    }
}

void GraphicManager::finishManeuver()
{
    maneuverFighterButtons.clear();
    validZoneIds.clear();
    maneuverSelectedFighter = nullptr;

    battle.addAction();
    if (battle.turnShouldEnd()) {
        battle.endTurnAndAdvance();
        beginTurnFlow();
    } else {
        setupState = SetupState::DONE;
        setupHandCards();
    }
}


void GraphicManager::setupDefenseCards(Fighter* defender, const std::vector<int>& defIndexes) {
    defenseCardsUI.clear();
    std::vector<Card>& hand = defender->gethand();
    
    float cardW = 120.f, cardH = 160.f, gap = 20.f;
    float totalW = defIndexes.size() * cardW + (defIndexes.empty() ? 0 : (defIndexes.size() - 1) * gap);
    float startX = (1280.f - totalW) / 2.f;
    float y = 720.f - cardH - 20.f;

    for (size_t i = 0; i < defIndexes.size(); i++) {
        int realIdx = defIndexes[i];
        CardWidget cw(font);
        float x = startX + i * (cardW + gap);

        cw.box.setSize({cardW, cardH});
        cw.box.setPosition({x, y});
        cw.box.setOutlineThickness(3.f);
        cw.box.setOutlineColor(sf::Color(200, 50, 50)); // حاشیه قرمز برای کارت‌های دفاع
        cw.usable = true;
        cw.handIndex = realIdx;

        Card& c = hand[realIdx];
        auto it = cardTextures.find(c.getcardname());
        if (it != cardTextures.end()) {
            cw.tex = &it->second;
            cw.box.setTexture(cw.tex);
            cw.box.setFillColor(sf::Color::White);
        } else {
            cw.box.setFillColor(sf::Color(230, 200, 200));
            cw.nameText.setString(c.getName());
            cw.nameText.setCharacterSize(16);
            cw.nameText.setFillColor(sf::Color::Black);
            cw.nameText.setPosition({x + 8.f, y + 8.f});
        }
        
        cw.valueText.setString("DEF:" + std::to_string(c.getValue()));
        cw.valueText.setCharacterSize(14);
        cw.valueText.setFillColor(cw.tex ? sf::Color::Yellow : sf::Color::Black);
        cw.valueText.setPosition({x + 6.f, y + cardH - 26.f});

        defenseCardsUI.push_back(std::move(cw));
    }
}

void GraphicManager::drawDefenseUI() {
    for (auto& cw : defenseCardsUI) {
        window.draw(cw.box);
        if (!cw.tex) window.draw(cw.nameText);
        window.draw(cw.valueText);
    }
    window.draw(cancelDefenseBox);
    window.draw(cancelDefenseText);
}

void GraphicManager::handleDefenseClick(sf::Vector2f pos) {
    if (cancelDefenseBox.getGlobalBounds().contains(pos)) {
        // کلیک روی ضربدر قرمز (عدم دفاع)
        battle.getCombat()->resolveCombat(pendingAttacker, pendingDefender, battle.getCurrentPlayer().getHero(), pendingAttackCardIndex, -1);
        finishAttack();
        return;
    }

    for (auto& cw : defenseCardsUI) {
        if (cw.box.getGlobalBounds().contains(pos)) {
            // کلیک روی یکی از کارت‌های دفاع
            battle.getCombat()->resolveCombat(pendingAttacker, pendingDefender, battle.getCurrentPlayer().getHero(), pendingAttackCardIndex, cw.handIndex);
            finishAttack();
            return;
        }
    }
}

void GraphicManager::finishAttack() {
    defenseCardsUI.clear();
    
    if (battle.hasExtraAction()) {
        battle.resetExtraAction();
        battle.resetActionCounter();
    } else {
        battle.addAction();
    }

    if (battle.turnShouldEnd()) {
        battle.endTurnAndAdvance();
        beginTurnFlow();
    } else {
        setupState = SetupState::DONE;
        setupHandCards();
    }
}


void GraphicManager::handleCardZoneClick(sf::Vector2f pos) {
    for (const auto& spot : boardSpots) {
        sf::Vector2f center = spot.circle.getPosition();
        float dx = pos.x - center.x, dy = pos.y - center.y;
        if (std::sqrt(dx * dx + dy * dy) > spot.circle.getRadius()) continue;
        
        bool valid = false;
        for (int id : validZoneIds) {
            if (id == spot.id) { valid = true; break; }
        }
        if (!valid) return;

        Player& current = battle.getCurrentPlayer();
        Player& other = battle.getOtherPlayer(current);

        if (pendingCard != nullptr && pendingCard->getEffect() != nullptr) {
            pendingCard->getEffect()->apply(pendingCardAttacker, other.getHero(), &battle, *pendingCard, spot.id);
            
            auto& hand = current.getHero()->gethand();
            for (size_t i = 0; i < hand.size(); ++i) {
                if (hand[i].getcardname() == pendingCard->getcardname()) {
                    hand.erase(hand.begin() + i);
                    break;
                }
            }
            battle.addAction();
        }

        pendingCard = nullptr;
        pendingCardAttacker = nullptr;
        validZoneIds.clear();

        if (battle.turnShouldEnd()) {
            battle.endTurnAndAdvance();
            beginTurnFlow();
        } else {
            setupState = SetupState::DONE;
            setupHandCards();
        }
        return;
    }
}