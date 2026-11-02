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
    sf::RenderWindow window(sf::VideoMode({ 1620, 720 }), "Mammal Tactics");
    sf::RenderWindow debug_window(sf::VideoMode({ 400, 400 }), "Debug Window");
    
    // Level setup
    Level Level1("Levels/Level1.txt");
    Level1.LoadLevel();
    int current_enemy = 0;

    // Saves setup
    SaveManager Save1;
    Save1.LoadFile();

    // Player setup
    Player player;

    // Background
    sf::Texture background_tex("Sprites/background.png");
    sf::RectangleShape background_shape({ 1620, 720 });
    background_shape.setTexture(&background_tex);

    // Vsync setup
    bool Vsync = true;
    window.setVerticalSyncEnabled(Vsync);

    // Text setup
    sf::Font Font1;
    if (!Font1.openFromFile("Cinzel-Bold.ttf"))
    {
        throw "Font could not be loaded";
    }

    const int text_size = 32;

    sf::Text TitleText(Font1, "Mammal Tactics", text_size);
    TitleText.setPosition({0, 650});

    // Button setup
    std::vector<Button> buttons;

    // Create text for the buttons
    sf::Text PlayText(Font1, "Play", text_size);
    sf::Text CreditText(Font1, "Credit", text_size);
    sf::Text QuitText(Font1, "Quit", text_size);
    sf::Text VsyncText(Font1, "VSync: On", text_size);
    sf::Text DecVolText(Font1, "Decrease", text_size);
    sf::Text IncVolText(Font1, "Increase", text_size);

    const int button_count = 6;
    sf::Text button_roles[button_count] = { PlayText, CreditText, QuitText, VsyncText, DecVolText, IncVolText };

    // Create every menu button
    for (int i = 0; i < button_count; i++)
    {
        // Makes button with location, colour, text 
        Button NewButton({ 550.f, 100.f * i + 200.f }, sf::Color::Green, button_roles[i]);
        buttons.push_back(NewButton);
    }

    // Display health of player and selected enemy
    sf::Text PlayerHealthText(Font1, "Player health: ", text_size);
    PlayerHealthText.setPosition({800, 0});

    sf::Text EnemyHealthText(Font1, "Enemy health: ", text_size);
    EnemyHealthText.setPosition({ 800, 100 });

    bool game_start = false; // false: menu open, True: Game open

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

            if (const auto* keyPressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                // Interactable menu if the game hasn't started
                if (!game_start)
                {
                    // Did user click on a button
                    for (int i = 0; i < button_count; i++)
                    {
                        // Menu
                        if (buttons[i].m_ButtonShape.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
                        {
                            // Start game
                            if (i == 0)
                            {
                                game_start = true;
                            }

                            // Credits
                            if (i == 1)
                            {

                            }

                            // End game
                            if (i == 2)
                            {
                                return 0;
                            }

                            // V-sync
                            if (i == 3)
                            {
                                Vsync = !Vsync;
                                window.setVerticalSyncEnabled(Vsync);

                                // Update the visual to the player
                                if (Vsync) { buttons[i].text.setString("Vsync: On"); }
                                else { buttons[i].text.setString("Vsync: Off"); }
                            }

                            // Increase music vol
                            if (i == 4)
                            {

                            }

                            // Decrease music vol
                            if (i == 4)
                            {

                            }
                        }
                    }
                }
                
                // Game has started
                else
                {
                    // Did user click on an enemy
                    for (int i = 0; i < Level1.enemies.size(); i++)
                    {
                        if (Level1.enemies[i].enemy_shape.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
                        {
                            current_enemy = i; // Target this enemy
                        }
                    }
                }
            }

            // Keybinds for the player
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
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
                    player.Attack(&Level1.enemies[current_enemy]);
                }

                // Player Movement
                player.Move(&Level1.enemies[current_enemy]);

                // Player special
                player.Special(&Level1.enemies[current_enemy]);
            }
        }

        // Animation events
        player.Animate();

        // Window events
        window.clear();
        
        // Draw Menu
        if (!game_start)
        {
            // Draw the text here
            window.draw(TitleText);

            // Draw menu buttons and text
            for (int i = 0; i < button_count; i++)
            {
                window.draw(buttons[i].m_ButtonShape);
                window.draw(buttons[i].text);
            }
        }

        // Draw game
        else
        {
            window.draw(background_shape);

            // Draw all level tiles
            for (int i = 0; i < Level1.level_tiles.size(); i++)
            {
                // Draw all specific tile type
                for (int j = 0; j < Level1.level_tiles[i].size(); j++)
                {
                    window.draw(*Level1.level_tiles[i][j]);
                }
            }

            // Draw the player and their range tiles
            for (int i = 0; i < player.range_tiles.size(); i++)
            {
                window.draw(player.range_tiles[i]);
            }
            window.draw(player.player_shape);

            // Draw all the enemies
            for (int i = 0; i < Level1.enemies.size(); i++)
            {
                window.draw(Level1.enemies[i].enemy_shape);
            }

            // Draw the text here
            window.draw(TitleText);

            // Draw health values of units
            PlayerHealthText.setString("Player Health: " + std::to_string(player.GetHealth()));
            window.draw(PlayerHealthText);

            EnemyHealthText.setString("Enemy (" + std::to_string(current_enemy) + ") Health: " + std::to_string(Level1.enemies[current_enemy].GetHealth()));
            window.draw(EnemyHealthText);
        }

        window.display();

        // Debug window events
        debug_window.clear();
        debug_window.display();
    }
}