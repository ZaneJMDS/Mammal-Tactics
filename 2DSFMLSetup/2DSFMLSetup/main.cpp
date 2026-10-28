/***********************************************************************
Author      :	Zane Jackson
Mail        :   Zane.Jackson@mds.ac.nz
Description :	Interval game (2D platformer)
File name   :   Main.cpp
**************************************************************************/

#include <SFML/Graphics.hpp>
#include <iostream> // For Debugging

#include "Level.h"
#include "SaveManager.h"
#include "Button.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1080, 720 }), "Mammal Tactics");
    sf::RenderWindow debug_window(sf::VideoMode({ 400, 400 }), "Debug Window");
    
    // Level setup
    Level Level1("Levels / Level1.txt");
    Level1.LoadLevel();

    // Saves setup
    SaveManager Save1;
    Save1.LoadFile();

    // Player setup
    Player player;

    // Enemy setup
    Enemy enemy;

    // Vsync setup
    bool Vsync = true;
    window.setVerticalSyncEnabled(true);

    // Button setup
    std::vector<Button> buttons;

    // Text setup
    sf::Font Font1;
    if (!Font1.openFromFile("Cinzel-Bold.ttf"))
    {
        throw "Font could not be loaded";
    }

    const int text_size = 32;

    sf::Text TitleText(Font1, "Mammal Tactics", text_size);
    TitleText.setPosition({0, 650});

    // Health of player and enemy
    sf::Text PlayerHealthText(Font1, "Player health: ", text_size);
    PlayerHealthText.setPosition({800, 0});

    sf::Text EnemyHealthText(Font1, "Enemy health: ", text_size);
    EnemyHealthText.setPosition({ 800, 100 });

    // Game loop
    while (window.isOpen())
    {
        // Only while window is selected
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) 
            {
                window.close();
            }

            // Catch the resize events
            if (const auto* resized = event->getIf<sf::Event::Resized>())
            {
                // update the view to the new size of the window
                sf::FloatRect visibleArea({ 0.f, 0.f }, sf::Vector2f(resized->size));
                window.setView(sf::View(visibleArea));
            }

            // Keybinds for the player
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                // Switch Vsync toggle
                if (keyPressed->code == sf::Keyboard::Key::V)
                {
                    Vsync = !Vsync;
                    window.setVerticalSyncEnabled(Vsync);

                    std::cout << "\nVSync: " << Vsync;
                }

                // Manual saving button
                if (keyPressed->code == sf::Keyboard::Key::Q)
                {
                    Save1.SaveFile();
                    std::cout << "\nLevels Beat: " << Save1.levels_beaten;
                }

                // Manual clear button
                if (keyPressed->code == sf::Keyboard::Key::C)
                {
                    Save1.ClearSave();
                    std::cout << "\nLevels Beat: " << Save1.levels_beaten;
                }

                // Manual attack button
                if (keyPressed->code == sf::Keyboard::Key::Space)
                {
                    player.Attack(&enemy);
                }

                // Player Movement
                player.Move(&enemy);
            }
        }

        // Window events
        window.clear();
        
        // Draw events
        // Draw all level tiles
        for (int i = 0; i < Level1.level_tiles.size(); i++)
        {
            // Draw all specific tile type
            for (int j = 0; j < Level1.level_tiles[i].size(); j++)
            {
                window.draw(*Level1.level_tiles[i][j]);
            }
        }

        // Draw the player
        window.draw(player.player_shape);

        // Draw the enemy
        window.draw(enemy.enemy_shape);

        // Draw the text here
        window.draw(TitleText);

        PlayerHealthText.setString("Player Health: " + std::to_string(player.GetHealth()));
        window.draw(PlayerHealthText);

        EnemyHealthText.setString("Enemy Health: " + std::to_string(enemy.GetHealth()));
        window.draw(EnemyHealthText);

        window.display();

        // Debug window events
        debug_window.clear();
        debug_window.display();
    }
}