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
    // Window setup
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Mammal Tactics", sf::State::Windowed); // GetDesktopMode resizes based on window size
    sf::RenderWindow debug_window(sf::VideoMode({ 400, 400 }), "Debug Window");
    
    // sf::VideoMode::getFullscreenMode();

    window.setFramerateLimit(60);
    window.setMinimumSize(sf::Vector2u{ 400, 400 }); // Can't decrease furthur than this

    // Level setup
    Level Level1("Levels/Level1.txt");
    Level1.LoadLevel();
    int current_enemy = 0;

    // Saves setup
    SaveManager Save1;
    Save1.LoadFile();

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
    sf::Text Credits(Font1, "Credits: \nGame by Zane\nMammal by Ryan", text_size);
    bool credits_visible = false;

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

    bool game_start = false; // false: menu open, True: Game open

    // Game loop
    while (window.isOpen())
    {
        // Set position of title text
        TitleText.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 8));

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
                                credits_visible = !credits_visible;
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
                    for (int i = 0; i < Level1.enemies.size(); i++)
                    {
                        // Did user click on an enemy
                        if (Level1.enemies[i].enemy_shape.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
                        {
                            // Change enemy target
                            Level1.enemies[current_enemy].UnsetTarget();
                            current_enemy = i;
                        }
                    }

                    // Did user click on an empty tile
                    for (int i = 0; i < Level1.level_back_tiles.size(); i++)
                    {
                        if (Level1.level_back_tiles[i]->getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window))))
                        {
                            // Move the player
                            Level1.players[0].MoveTo(sf::Vector2f(sf::Mouse::getPosition(window)));
                        }
                    }
                }
            }

            Level1.enemies[current_enemy].SetTarget();

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
                    Level1.players[0].Attack(&Level1.enemies[current_enemy]);
                }

                // Increase resolution
                if (keyPressed->code == sf::Keyboard::Key::Add)
                {
                    window.setSize({ window.getSize().x + 100, window.getSize().y + 100 });
                }

                // Decrease resolution
                if (keyPressed->code == sf::Keyboard::Key::Subtract)
                {
                    window.setSize({ window.getSize().x - 100, window.getSize().y - 100 });
                }

                // Game menu and play toggle
                if (keyPressed->code == sf::Keyboard::Key::Escape)
                {
                    game_start = !game_start;
                }

                // Player Movement
                Level1.players[0].Move(&Level1.enemies[current_enemy]);

                // TODO Move outside of keypressed to go with new mouse move input 
                int k = 0;
                // Check for collisions between player range tiles
                for (int i = 0; i < 9; i++)
                {
                    // AND Wall tiles
                    for (int j = 0; j < Level1.wall_tiles.size(); j++)
                    {
                        // This tile is colliding with this wall
                        if (Level1.players[0].range_tiles[i].getGlobalBounds().findIntersection(Level1.wall_tiles[j]->getGlobalBounds()))
                        {
                            Level1.players[0].range_tiles[i].setFillColor(sf::Color(0, 0, 0, 0));
                        }

                        // This tile isn't colliding with any wall
                        else if (k+1 == Level1.wall_tiles.size())
                        {
                            Level1.players[0].range_tiles[i].setFillColor(sf::Color(0, 0, 255, 128)); // Light Blue
                        }

                        // Not colliding with this wall
                        else
                        {
                            k++;
                        }
                    }
                    k = 0;
                }

                // Wall tiles
                for (int i = 0; i < Level1.wall_tiles.size(); i++)
                {
                    // With player
                    if (Level1.players[0].player_shape.getGlobalBounds().findIntersection(Level1.wall_tiles[i]->getGlobalBounds()))
                    {
                        Level1.players[0].player_shape.move({ -64, 0 });
                    }
                }

                // Player special
                Level1.players[0].Special(&Level1.enemies[current_enemy]);

                // Check to see if any enemies have health, if not proceed to next level
                int enemies_dead = 0;
                for (int i = 0; i < Level1.enemies.size(); i++)
                {
                    if (Level1.enemies[i].GetHealth() <= 0)
                    {
                        enemies_dead++;
                    }

                    // All enemies killed
                    if (enemies_dead == Level1.enemies.size())
                    {
                        std::cout << "You win, load next level";
                    }
                }
            }
        }

        // Animation events
        Level1.players[0].Animate();
        Level1.players[0].Display();

        // Window events
        window.clear();
        
        // Draw Menu
        if (!game_start)
        {
            // Draw the text here
            window.draw(TitleText);

            // Draw menu buttons
            if (!credits_visible)
            {
                // Draw menu buttons and text
                for (int i = 0; i < button_count; i++)
                {
                    window.draw(buttons[i].m_ButtonShape);
                    window.draw(buttons[i].text);
                }
            }

            // Draw credits
            else
            {
                window.draw(Credits);
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
            for (int i = 0; i < Level1.players[0].range_tiles.size(); i++)
            {
                window.draw(Level1.players[0].range_tiles[i]);
            }
            window.draw(Level1.players[0].player_shape); 
            window.draw(Level1.players[0].PlayerText); // Might have to store this in the level file

            // Draw all the enemies
            for (int i = 0; i < Level1.enemies.size(); i++)
            {
                window.draw(Level1.enemies[i].enemy_shape);
            }
        }

        window.display();

        // Debug window events
        debug_window.clear();
        debug_window.setVisible(false);
        debug_window.display();
    }
}