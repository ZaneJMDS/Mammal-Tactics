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
    sf::RenderWindow window(sf::VideoMode({ 1620, 720 }), "Mammal Tactics", sf::State::Windowed); // GetDesktopMode resizes based on window size
    sf::RenderWindow debug_window(sf::VideoMode({ 400, 400 }), "Debug Window");
    
    // sf::VideoMode::getDesktopMode()
    // sf::VideoMode::getFullscreenMode();

    window.setFramerateLimit(60);
    window.setMinimumSize(sf::Vector2u{ 400, 400 }); // Can't decrease furthur than this

    // Level setup
    Level level1("Levels/Level1.txt");
    Level level2("Levels/Level2.txt");
    Level level3("Levels/Level3.txt");
    const int max_levels = 3;
    Level levels[max_levels] = { level1, level2, level3 };
    
    int current_level = 0;
    levels[current_level].LoadLevel();

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
    sf::Font Font1("Cinzel-Bold.ttf");

    const int text_size = 32;
    sf::Text TitleText(Font1, "Mammal Tactics", text_size);
    sf::Text Credits(Font1, "Credits: \nGame by Zane\nMammal by Ryan", text_size);
    sf::Text TotalTurns(Font1, "Total turns: ", text_size);
    bool credits_visible = false;

    // Button setup
    std::vector<Button> buttons;

    // Menu buttons
    sf::Text PlayText(Font1, "Play", text_size);
    sf::Text CreditText(Font1, "Credit", text_size);
    sf::Text QuitText(Font1, "Quit", text_size);
    sf::Text VsyncText(Font1, "VSync: On", text_size);
    sf::Text DecVolText(Font1, "Decrease", text_size);
    sf::Text IncVolText(Font1, "Increase", text_size);

    // Gameplay buttons
    sf::Text PassTurnText(Font1, "Pass turn", text_size); // Gameplay button

    // Debug buttons
    sf::Text InfiniteTurnText(Font1, "Infinite turn: Off", text_size);

    const int button_count = 6;
    sf::Text button_roles[button_count] = { PlayText, CreditText, QuitText, VsyncText, DecVolText, IncVolText };

    // Other Text
    sf::Text TurnText(Font1, "Turns: ", text_size);

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
                    for (int i = 0; i < buttons.size(); i++)
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
                            if (i == 5)
                            {

                            }
                        }
                    }
                }
                
                // Game has started
                else
                {
                    sf::Vector2f mouse_pos = sf::Vector2f(sf::Mouse::getPosition(window));
                    levels[current_level].Click(mouse_pos);
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
                    levels[current_level].Combat();
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

                // Pass turn
                if (keyPressed->code == sf::Keyboard::Key::P)
                {
                    levels[current_level].EnemyTurn();
                }

                // Check to see if any enemies have health, if not proceed to next level
                int enemies_dead = 0;
                for (int i = 0; i < levels[current_level].enemies.size(); i++)
                {
                    if (levels[current_level].enemies[i].GetHealth() <= 0)
                    {
                        enemies_dead++;
                    }

                    // All enemies killed, move to next level
                    if (enemies_dead == levels[current_level].enemies.size())
                    {
                        levels[current_level].UnloadLevel();

                        // Move to next level
                        if (current_level < max_levels - 1)
                        {
                            current_level++;
                            levels[current_level].LoadLevel();
                        }

                        // Return to menu
                        else
                        {
                            game_start = false;
                            std::string turns_string = "Max turns:\n";

                            // Display scores to the user
                            for (int i = 0; i < max_levels; i++)
                            {
                                turns_string += "Level " + std::to_string(i + 1) + ": " + std::to_string(levels[i].GetTurn()) + "\n";
                            }

                            TotalTurns.setString(turns_string);
                        }
                    }
                }
            }
        }

        // Debug window events
        while (const std::optional event = window.pollEvent())
        {
            // Catch the resize events
            if (const auto* resized = event->getIf<sf::Event::Resized>())
            {
                // update the view to the new size of the window
                sf::FloatRect visibleArea({ 0.f, 0.f }, sf::Vector2f(resized->size));
                debug_window.setView(sf::View(visibleArea));
            }

            // Keybinds for the player
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                // Infinite turns toggle
                if (keyPressed->code == sf::Keyboard::Key::I)
                {
                    levels[current_level].infinite_turn = !levels[current_level].infinite_turn;
                }
            }
        }

        levels[current_level].Update();

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

                window.draw(TotalTurns);
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

            // Draw the current turn
            TurnText.setPosition({1000, 0});
            TurnText.setString("Turns: " + std::to_string(levels[current_level].GetTurn()));
            window.draw(TurnText);

            levels[current_level].Draw(window);
        }

        window.display();

        // Debug window events
        debug_window.clear();
        InfiniteTurnText.setString("Infinite turns: " + levels[current_level].infinite_turn);
        debug_window.draw(InfiniteTurnText);
        debug_window.display();
    }
}