#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <vector>
#include <string>
#include <optional>

class Intro
{
public:
    Intro();

    // اجرای انیمیشن شروع بازی
    bool run(sf::RenderWindow& window);

private:

    // -----------------------------
    // Images
    // -----------------------------

    std::vector<sf::Texture> textures;
    std::vector<sf::Sprite> sprites;

    sf::Font font;


    // -----------------------------
    // Audio
    // -----------------------------

    sf::Music introMusic;

    sf::SoundBuffer typeBuffer;
    sf::SoundBuffer crackBuffer;

    std::optional<sf::Sound> typeSound;
    std::optional<sf::Sound> crackSound;


    // -----------------------------
    // Functions
    // -----------------------------

    void loadImages();
    void loadSounds();

    void drawImages(
        sf::RenderWindow& window,
        int index,
        float alpha
    );

    void drawTitle(
        sf::RenderWindow& window,
        float alpha
    );

    void drawCrack(
        sf::RenderWindow& window,
        float progress
    );

    void centerSprite(
        sf::Sprite& sprite,
        sf::Vector2u windowSize
    );
};