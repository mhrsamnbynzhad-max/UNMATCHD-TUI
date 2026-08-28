#include "intro.h"

#include <iostream>
#include <cmath>

Intro::Intro()
{
    loadImages();
    loadSounds();

    if (!font.openFromFile("player.ttf"))
    {
        std::cerr << "ERROR: Could not load player.ttf\n";
    }
}

void Intro::loadImages()
{
    const std::vector<std::string> imagePaths =
    {
        "assets/intro/scene1.jpeg",
        "assets/intro/scene2.jpeg",
        "assets/intro/scene3.jpeg",
        "assets/intro/scene4.jpeg",
        "assets/intro/scene5.jpeg",
        "assets/intro/scene6.jpeg"
    };

    for (const auto& path : imagePaths)
    {
        sf::Texture texture;

        if (!texture.loadFromFile(path))
        {
            std::cerr << "ERROR: Could not load " << path << "\n";
            continue;
        }

        textures.push_back(std::move(texture));
    }

    for (auto& texture : textures)
    {
        sf::Sprite sprite(texture);
        sprites.push_back(std::move(sprite));
    }
}

void Intro::centerSprite(
    sf::Sprite& sprite,
    sf::Vector2u windowSize)
{
    sf::FloatRect bounds = sprite.getLocalBounds();

    sprite.setOrigin(
        sf::Vector2f(
            bounds.position.x + bounds.size.x / 2.f,
            bounds.position.y + bounds.size.y / 2.f
        )
    );

    sprite.setPosition(
        sf::Vector2f(
            windowSize.x / 2.f,
            windowSize.y / 2.f
        )
    );

    // تصویر را به اندازه صفحه می‌کنیم
    float scaleX =
        static_cast<float>(windowSize.x) /
        bounds.size.x;

    float scaleY =
        static_cast<float>(windowSize.y) /
        bounds.size.y;

    float scale = std::max(scaleX, scaleY);

    sprite.setScale(
        sf::Vector2f(scale, scale)
    );
}

void Intro::drawImages(
    sf::RenderWindow& window,
    int index,
    float alpha)
{
    if (index < 0 ||
        index >= static_cast<int>(sprites.size()))
    {
        return;
    }

    sf::Sprite& sprite = sprites[index];

    centerSprite(
        sprite,
        window.getSize()
    );

    sprite.setColor(
        sf::Color(
            255,
            255,
            255,
            static_cast<std::uint8_t>(alpha)
        )
    );

    window.draw(sprite);
}

void Intro::drawTitle(
    sf::RenderWindow& window,
    float alpha)
{
    sf::Text title(font);

    title.setString("UNMATCHED");

    title.setCharacterSize(100);

    title.setStyle(sf::Text::Bold);

    title.setFillColor(
        sf::Color(
            220,
            190,
            100,
            static_cast<std::uint8_t>(alpha)
        )
    );

    title.setOutlineThickness(4.f);

    title.setOutlineColor(
        sf::Color(
            50,
            10,
            10,
            static_cast<std::uint8_t>(alpha)
        )
    );

    sf::FloatRect bounds =
        title.getLocalBounds();

    title.setOrigin(
        sf::Vector2f(
            bounds.position.x + bounds.size.x / 2.f,
            bounds.position.y + bounds.size.y / 2.f
        )
    );

    title.setPosition(
        sf::Vector2f(
            window.getSize().x / 2.f,
            window.getSize().y / 2.f
        )
    );

    window.draw(title);
}








void Intro::loadSounds()
{
    if (!introMusic.openFromFile(
        "assets/music/intro.mp3"))
    {
        std::cerr << "ERROR: Could not load intro.mp3\n";
    }

    introMusic.setLooping(false);


    if (!typeBuffer.loadFromFile(
        "assets/music/type.mp3"))
    {
        std::cerr << "ERROR: Could not load type.mp3\n";
    }


    if (!crackBuffer.loadFromFile(
        "assets/music/crack.mp3"))
    {
        std::cerr << "ERROR: Could not load crack.mp3\n";
    }
}






void Intro::drawCrack(
    sf::RenderWindow& window,
    float progress)
{
    if (progress <= 0.f)
        return;

    sf::Vector2u size = window.getSize();

    sf::Vector2f center(
        size.x / 2.f,
        size.y / 2.f
    );

    float length =
        40.f + progress * 250.f;

    std::vector<sf::Vertex> cracks;

    sf::Vertex v1;
    v1.position = center;
    v1.color = sf::Color::White;

    sf::Vertex v2;
    v2.position =
        center + sf::Vector2f(
            -length,
            -length * 0.4f
        );
    v2.color = sf::Color::White;

    cracks.push_back(v1);
    cracks.push_back(v2);


    // ترک سمت راست
    sf::Vertex v3;
    v3.position = center;
    v3.color = sf::Color::White;

    sf::Vertex v4;
    v4.position =
        center + sf::Vector2f(
            length,
            -length * 0.3f
        );
    v4.color = sf::Color::White;

    cracks.push_back(v3);
    cracks.push_back(v4);


    // ترک پایین
    sf::Vertex v5;
    v5.position = center;
    v5.color = sf::Color::White;

    sf::Vertex v6;
    v6.position =
        center + sf::Vector2f(
            -length * 0.2f,
            length
        );
    v6.color = sf::Color::White;

    cracks.push_back(v5);
    cracks.push_back(v6);


    // ترک بالا
    sf::Vertex v7;
    v7.position = center;
    v7.color = sf::Color::White;

    sf::Vertex v8;
    v8.position =
        center + sf::Vector2f(
            length * 0.2f,
            -length
        );
    v8.color = sf::Color::White;

    cracks.push_back(v7);
    cracks.push_back(v8);


    window.draw(
        cracks.data(),
        cracks.size(),
        sf::PrimitiveType::Lines
    );
}




bool Intro::run(sf::RenderWindow& window)
{
    sf::Clock clock;

    // -----------------------------
    // موسیقی اول
    // -----------------------------
    introMusic.setVolume(60.f);
    introMusic.play();

    typeSound.emplace(typeBuffer);
    crackSound.emplace(crackBuffer);

    bool typePlayed = false;
    bool crackPlayed = false;

    while (window.isOpen())
    {
        // =============================
        // Event
        // =============================

        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                introMusic.stop();
                window.close();
                return false;
            }

            if (event->is<sf::Event::KeyPressed>())
            {
                const auto* key =
                    event->getIf<sf::Event::KeyPressed>();

                if (key &&
                    key->code == sf::Keyboard::Key::Escape)
                {
                    introMusic.stop();
                    typeSound->stop();
                    crackSound->stop();

                    return true;
                }
            }

            if (event->is<sf::Event::MouseButtonPressed>())
            {
                introMusic.stop();
                typeSound->stop();
                crackSound->stop();

                return true;
            }
        }


        float time =
            clock.getElapsedTime().asSeconds();


        // =============================
        // ۶ عکس
        // =============================

        if (time < 12.f)
        {
            int index =
                static_cast<int>(time / 2.f);

            float localTime =
                std::fmod(time, 2.f);

            float alpha = 255.f;

            if (localTime < 0.4f)
            {
                alpha =
                    (localTime / 0.4f) * 255.f;
            }

            if (localTime > 1.6f)
            {
                alpha =
                    ((2.f - localTime) / 0.4f)
                    * 255.f;
            }

            window.clear(sf::Color::Black);

            drawImages(
                window,
                index,
                alpha
            );

            window.display();

            continue;
        }


        // =============================
        // موسیقی اول تمام شد
        // موسیقی دوم شروع شود
        // =============================

        if (time >= 12.f &&
            !typePlayed)
        {
            introMusic.stop();

            typeSound->play();

            typePlayed = true;
        }


        // =============================
        // اسم بازی
        // =============================

        if (time < 15.f)
        {
            window.clear(sf::Color::Black);

            float elapsed =
                time - 12.f;

            std::string fullText =
                "UNMATCHED";

            int letters =
                static_cast<int>(
                    elapsed / 0.15f
                );

            if (letters >
                static_cast<int>(fullText.size()))
            {
                letters =
                    fullText.size();
            }

            std::string current =
                fullText.substr(
                    0,
                    letters
                );

            sf::Text title(font);

            title.setString(current);

            title.setCharacterSize(100);

            title.setStyle(
                sf::Text::Bold
            );

            title.setFillColor(
                sf::Color(220, 190, 100)
            );

            title.setOutlineThickness(4.f);

            title.setOutlineColor(
                sf::Color(50, 10, 10)
            );

            sf::FloatRect bounds =
                title.getLocalBounds();

            title.setOrigin(
                sf::Vector2f(
                    bounds.position.x +
                    bounds.size.x / 2.f,
                    bounds.position.y +
                    bounds.size.y / 2.f
                )
            );

            title.setPosition(
                sf::Vector2f(
                    window.getSize().x / 2.f,
                    window.getSize().y / 2.f
                )
            );

            window.draw(title);

            window.display();

            continue;
        }


        // =============================
        // موسیقی دوم قطع
        // موسیقی سوم شروع
        // =============================

        if (time >= 15.f &&
            !crackPlayed)
        {
            typeSound->stop();

            crackSound->play();

            crackPlayed = true;
        }


        // =============================
        // ترک خوردن
        // =============================

        if (time < 17.f)
        {
            window.clear(sf::Color::Black);

            drawTitle(
                window,
                255.f
            );

            float progress =
                (time - 15.f) / 2.f;

            drawCrack(
                window,
                progress
            );

            window.display();

            continue;
        }


        // =============================
        // پایان
        // =============================

        introMusic.stop();
        typeSound->stop();
        crackSound->stop();

        return true;
    }

    return false;
}