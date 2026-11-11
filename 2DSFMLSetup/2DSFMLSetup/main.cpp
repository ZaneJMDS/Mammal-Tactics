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

bool game_start = false; // false: menu open, True: Game open

// Level setup
Level level1("Levels/Level1.txt");
Level level2("Levels/Level2.txt");
Level level3("Levels/Level3.txt");
const int max_levels = 3;
Level levels[max_levels] = { level1, level2, level3 };

// Saves setup
SaveManager Save1;
int current_level = Save1.LoadFile();

// Move between levels
static void SwapLevel(int _target_level)
{
    if (!game_start) { game_start = true; }
    levels[current_level].UnloadLevel();
    current_level = _target_level;
    levels[current_level].LoadLevel();
    levels[current_level].Reset();
    Save1.SaveFile(current_level);
}

int main()
{
    // Window setup
    sf::RenderWindow window(sf::VideoMode({ 1620, 720 }), "Mammal Tactics", sf::State::Windowed); // GetDesktopMode resizes based on window size
    sf::RenderWindow debug_window(sf::VideoMode({ 400, 400 }), "Debug Window");
    
    // sf::VideoMode::getDesktopMode()
    // sf::VideoMode::getFullscreenMode();

    window.setFramerateLimit(60);
    window.setMinimumSize(sf::Vector2u{ 400, 400 }); // Can't decrease furthur than this
    
    levels[current_level].LoadLevel();

    // Background
    sf::Texture background_tex("Sprites/background.png");
    sf::RectangleShape background_shape({ 1620, 720 });
    background_shape.setTexture(&background_tex);

    // Vsync setup
    bool Vsync = true;
    window.setVerticalSyncEnabled(Vsync);

    // Text setup
    sf::Font Font1("Cinzel-Bold.ttf");

    const int text_size = 24;
    sf::Text TitleText(Font1, "Mammal Tactics", text_size*2);
    sf::Text Credits(Font1, "Credits: \nGame by Zane\nMammal by Ryan", text_size);
    sf::Text TotalTurns(Font1, "Total turns: ", text_size);
    bool credits_visible = false;

    // Button setup
    std::vector<Button> menu_buttons;
    std::vector<Button> gameplay_buttons;
    std::vector<Button> debug_buttons;

    // Menu buttons
    sf::Text PlayText(Font1, "Play", text_size);
    sf::Text CreditText(Font1, "Credit", text_size);
    sf::Text QuitText(Font1, "Quit", text_size);
    sf::Text VsyncText(Font1, "VSync: On", text_size);
    sf::Text ClearSave(Font1, "Clear save", text_size);
    sf::Text DecVolText(Font1, "Decrease", text_size);
    sf::Text IncVolText(Font1, "Increase", text_size);

    const int menu_button_count = 7;
    sf::Text menu_button_roles[menu_button_count] = { PlayText, CreditText, QuitText, VsyncText, ClearSave, DecVolText, IncVolText };

    // Gameplay buttons
    sf::Text PassTurnText(Font1, "Pass turn", text_size);
    sf::Text AttackText(Font1, "Attack", text_size);
    sf::Text SpecialText(Font1, "Special", text_size);

    sf::Text gameplay_button_roles[3] = { PassTurnText, AttackText, SpecialText };

    // Debug buttons
    sf::Text InfiniteTurnText(Font1, "Infinite turn: Off", 20);
    sf::Text PreviousTurnText(Font1, "Previous level", 20);
    sf::Text NextTurnText(Font1, "Next level", 20);

    sf::Text debug_button_roles[3] = { InfiniteTurnText, PreviousTurnText, NextTurnText };

    // Other Text
    sf::Text TurnText(Font1, "Turns: ", text_size);
    sf::Text LevelText(Font1, "Levels: ", text_size);

    // Create every menu button
    for (int i = 0; i < menu_button_count; i++)
    {
        // Makes button with location, colour, text 
        Button NewButton({ 550.f, 100.f * i + 200.f }, sf::Color::Green, menu_button_roles[i]);
        menu_buttons.push_back(NewButton);
    }

    // Create every gameplay button
    for (int i = 0; i < 3; i++)
    {
        // Makes button with location, colour, text 
        Button NewButton({ 150.f * i, 650.f }, sf::Color::Red, gameplay_button_roles[i]);
        gameplay_buttons.push_back(NewButton);
    }

    // Create every debug button
    for (int i = 0; i < 3; i++)
    {
        // Makes button with location, colour, text 
        Button NewButton({ 0.f, 0.f + 100.f * i }, sf::Color::Blue, debug_button_roles[i]);
        debug_buttons.push_back(NewButton);
    }

    // Game loop
    while (window.isOpen())
    {
        // Set position of title text
        TitleText.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 8));

        // Update mouse pos
        sf::Vector2f mouse_pos = sf::Vector2f(sf::Mouse::getPosition(window));

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
                    for (int i = 0; i < menu_buttons.size(); i++)
                    {
                        // Menu
                        if (menu_buttons[i].m_ButtonShape.getGlobalBounds().contains(mouse_pos))
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
                                if (Vsync) { menu_buttons[i].text.setString("Vsync: On"); }
                                else { menu_buttons[i].text.setString("Vsync: Off"); }
                            }

                            // Clear save
                            if (i == 4)
                            {
                                Save1.ClearSave();
                            }

                            // Increase music vol
                            if (i == 5)
                            {

                            }

                            // Decrease music vol
                            if (i == 6)
                            {

                            }
                        }
                    }
                }
                
                // Game has started
                else
                {
                    levels[current_level].Click(mouse_pos);

                    // Did user click on a button
                    for (int i = 0; i < gameplay_buttons.size(); i++)
                    {

                        // Menu
                        if (gameplay_buttons[i].m_ButtonShape.getGlobalBounds().contains(mouse_pos))
                        {
                            // Pass turn
                            if (i == 0)
                            {
                                levels[current_level].PassTurn();
                            }

                            // Attack
                            if (i == 1)
                            {
                                levels[current_level].Combat();
                            }

                            // Special
                            if (i == 2)
                            {
                                levels[current_level].PlayerSpecial();
                            }
                        }
                    }
                }
            }

            // Keybinds for the player
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
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

                // Player special
                if (keyPressed->code == sf::Keyboard::Key::S)
                {
                    levels[current_level].PlayerSpecial();
                }
            }
        }

        // Debug window events
        while (const std::optional event = debug_window.pollEvent())
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
                    levels[current_level].ToggleInfiniteTurn();
                }

                // Level skip
                // Debug Level switcher
                for (int i = 0; i < max_levels; i++)
                {
                    // Dont unload the level if it is current one 
                    if (i != current_level)
                    {
                        // Ascii values for number keys
                        if (keyPressed->code == sf::Keyboard::Key(i + 27))
                        {
                            SwapLevel(i);
                        }
                    }
                }
            }

            // Debug Buttons
            if (const auto* keyPressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                // Did user click on a button
                for (int i = 0; i < gameplay_buttons.size(); i++)
                {
                    // Menu
                    if (debug_buttons[i].m_ButtonShape.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(debug_window))))
                    {
                        // Infinite turn
                        if (i == 0)
                        {
                            levels[current_level].ToggleInfiniteTurn();
                        }

                        // Previous level
                        if (i == 1)
                        {
                            if (current_level > 0)
                            {
                                SwapLevel(current_level - 1);
                            }
                        }

                        // Next level
                        if (i == 2)
                        {
                            if (current_level < max_levels - 1)
                            {
                                SwapLevel(current_level + 1);
                            }
                        }
                    }
                }
            }
        }

        levels[current_level].Update();

        // All enemies killed, move to next level
        if (!levels[current_level].CheckAllEnemiesAlive())
        {
            // Move to next level
            if (current_level < max_levels - 1)
            {
                SwapLevel(current_level + 1);
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
                for (int i = 0; i < menu_button_count; i++)
                {
                    window.draw(menu_buttons[i].m_ButtonShape);
                    window.draw(menu_buttons[i].text);
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
            TurnText.setPosition({1000.f, 0.f});
            TurnText.setString("Turns: " + std::to_string(levels[current_level].GetTurn()));
            window.draw(TurnText);

            // Draw the current level
            LevelText.setPosition({ 0.f, 800.f });
            LevelText.setString("Level: " + std::to_string(current_level + 1));
            window.draw(LevelText);

            levels[current_level].Draw(window);

            // Draw gameplay buttons
            for (int i = 0; i < gameplay_buttons.size(); i++)
            {
                window.draw(gameplay_buttons[i].m_ButtonShape);
                window.draw(gameplay_buttons[i].text);
            }
        }

        window.display();

        // Debug window events
        debug_window.clear();

        // Update button text
        if (levels[current_level].infinite_turn)
        {
            debug_buttons[0].text.setString("Infinite turn : On");
        }

        else
        {
            debug_buttons[0].text.setString("Infinite turn: Off");
        }

        // Draw debug buttons
        for (int i = 0; i < debug_buttons.size(); i++)
        {
            debug_window.draw(debug_buttons[i].m_ButtonShape);
            debug_window.draw(debug_buttons[i].text);
        }

        debug_window.display();
    }
}