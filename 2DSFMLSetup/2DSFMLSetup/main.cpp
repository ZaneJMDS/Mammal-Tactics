/***********************************************************************
Author      :	Zane Jackson
Description :	Mammal Tactics (Turn based RPG)
File name   :   Main.cpp
**************************************************************************/

#include <SFML/Graphics.hpp>
#include <iostream> // For Debugging

#include "Level.h"
#include "SaveManager.h"
#include "Button.h"

bool game_start = false; // false: menu open, True: Game open
bool Vsync = true; 

// Level setup
Level level1("Levels/Level1.txt");
Level level2("Levels/Level2.txt");
Level level3("Levels/Level3.txt");
const int max_levels = 3;
Level levels[max_levels] = { level1, level2, level3 };

// Saves setup
SaveManager Save1;
int current_level = 0;
std::vector<int> best_turns;

// Move between levels
static void SwapLevel(int _target_level, sf::Vector2u _resolution)
{
    if (!game_start) { game_start = true; }

    levels[current_level].UnloadLevel();
    best_turns[current_level] = levels[current_level].GetBestTurns();

    current_level = _target_level;
    levels[current_level].LoadLevel();
    levels[current_level].Reset();
    Save1.SaveFile(current_level, Vsync, _resolution, best_turns);
}

int main()
{
    // Load info from save file
    Save1.LoadFile();
    current_level = Save1.GetCurrentLevel();
    Vsync = Save1.GetVsync();
    best_turns = Save1.GetLevelTurns();

    // View settings
    sf::Vector2f Center(1000, 1000);
    sf::Vector2f HalfSize(400, 300);
    sf::View main_view(Center, HalfSize);

    // Window setup
    sf::RenderWindow window(sf::VideoMode(Save1.GetResolution()), "Mammal Tactics", sf::State::Windowed); // GetDesktopMode resizes based on window size
    window.setView(main_view);
    sf::RenderWindow debug_window(sf::VideoMode({ 400, 400 }), "Debug Window");
    
    // sf::VideoMode::getDesktopMode()
    // sf::VideoMode::getFullscreenMode();

    window.setFramerateLimit(60);
    window.setMinimumSize(sf::Vector2u{ 400, 400 }); // Can't decrease furthur than this
    
    levels[current_level].LoadLevel(); // Load the current file level

    // Background
    sf::Texture background_tex("Sprites/background.png");
    sf::RectangleShape background_shape;
    background_shape.setTexture(&background_tex);

    // Vsync setup
    window.setVerticalSyncEnabled(Vsync);

    // Text setup
    sf::Font Font1("Cinzel-Bold.ttf");
    const int text_size = 24;

    // Menu text
    sf::Text TitleText(Font1, "Mammal Tactics", text_size*2);
    TitleText.setOrigin(TitleText.getLocalBounds().getCenter());
    sf::Text Credits(Font1, "Credits: \nGame by Zane\nMammal by Ryan\nIcons by Hilmy Abiyyu A, meaicon, BomSymbols", text_size);
    sf::Text TotalTurns(Font1, "Total turns: ", text_size);
    
    // Gameplay text
    sf::Text TurnText(Font1, "Turns: ", text_size);
    sf::Text LevelText(Font1, "Levels: ", text_size);
    sf::Text CompletionText(Font1, "Player wins", text_size);

    bool credits_visible = false;

    // Button setup
    std::vector<Button> menu_buttons;
    std::vector<Button> gameplay_buttons;
    std::vector<Button> debug_buttons;

    const int menu_button_count = 6;
    std::string menu_button_roles[menu_button_count] = { "Play", "Credit", "Quit", "VSync: On", "Clear save", "Menu" };
    std::string gameplay_button_roles[3] = { "Pass turn", "Attack", "Special" };
    std::string debug_button_roles[3] = { "Infinite turn: Off", "Prev level", "Next level" };

    // Create every menu button
    for (int i = 0; i < menu_button_count; i++)
    {
        // Makes button with location, colour, text 
        Button NewButton({ 550.f, 100.f * i + 200.f }, sf::Color::Green, menu_button_roles[i]);
        NewButton.text.setFont(Font1);
        menu_buttons.push_back(NewButton);
    }

    // Create every gameplay button
    for (int i = 0; i < 3; i++)
    {
        // Makes button with location, colour, text 
        Button NewButton({ 150.f * i, 650.f }, sf::Color::Red, gameplay_button_roles[i]);
        NewButton.text.setFont(Font1);
        gameplay_buttons.push_back(NewButton);
    }

    // Create every debug button
    for (int i = 0; i < 3; i++)
    {
        // Makes button with location, colour, text 
        Button NewButton({ 0.f, 0.f + 100.f * i }, sf::Color::Blue, debug_button_roles[i]);
        NewButton.text.setFont(Font1);
        debug_buttons.push_back(NewButton);
    }

    // Game loop
    while (window.isOpen())
    {
        // Set position and size relative to the window
        TitleText.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 8));
        Credits.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
        CompletionText.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));

        background_shape.setSize(sf::Vector2f(window.getSize()));

        // Update mouse pos
        sf::Vector2f mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

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

                                // Restart from the beggining if player has beaten every level
                                if (current_level == max_levels)
                                {
                                    SwapLevel(0, window.getSize());
                                }
                            }

                            // Credits
                            if (i == 1)
                            {
                                credits_visible = !credits_visible;
                            }

                            // End game
                            if (i == 2)
                            {
                                Save1.SaveFile(current_level, Vsync, window.getSize(), best_turns);
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

                                Save1.SaveFile(current_level, Vsync, window.getSize(), best_turns);
                            }

                            // Clear save
                            if (i == 4)
                            {
                                Save1.ClearSave();
                            }

                            // Return to menu
                            if (i == 5)
                            {
                                credits_visible = false;
                                game_start = false;
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
                                SwapLevel(current_level - 1, window.getSize());
                            }
                        }

                        // Next level
                        if (i == 2)
                        {
                            if (current_level < max_levels - 1)
                            {
                                SwapLevel(current_level + 1, window.getSize());
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
            CompletionText.setString("Player wins");

            // Move to next level
            if (current_level < max_levels - 1)
            {
                SwapLevel(current_level + 1, window.getSize());
            }

            // Return to menu
            else
            {
                game_start = false;
            }
        }

        // All players killed return to menu
        if (!levels[current_level].CheckAllPlayersAlive())
        {
            CompletionText.setString("Enemy wins");
            game_start = false;
        }

        // Display fastest completion to the user
        std::string turns_string = "Fastest completion:\n";

        for (int i = 0; i < best_turns.size(); i++)
        {
            if (levels[i].GetBestTurns() < best_turns[i])
            {
                best_turns[i] = levels[i].GetBestTurns();
            }

            turns_string += "Level " + std::to_string(i + 1) + ": " + std::to_string(best_turns[i]) + " turns\n";
        }

        TotalTurns.setString(turns_string);


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
                for (int i = 0; i < menu_button_count - 1; i++)
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

                // Draw the menu button
                window.draw(menu_buttons[menu_button_count - 1].m_ButtonShape);
                window.draw(menu_buttons[menu_button_count - 1].text);
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

            if (!levels[current_level].CheckAllEnemiesAlive() || !levels[current_level].CheckAllPlayersAlive())
            {
                window.draw(CompletionText);
            }

            // Draw the menu button
            window.draw(menu_buttons[menu_button_count - 1].m_ButtonShape);
            window.draw(menu_buttons[menu_button_count - 1].text);
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