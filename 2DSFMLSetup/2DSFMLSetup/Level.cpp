#include "level.h"

Level::Level(std::string _filepath) : filepath(_filepath)
{
	if (!player_tex.loadFromFile("Sprites/player.png"))
	{
		throw "Can't find player texture";
	}

	// Icons
	if (!attack_icon_tex.loadFromFile("Sprites/attack.png"))
	{
		throw "Can't find icon";
	}

	if (!footstep_icon_tex.loadFromFile("Sprites/footstep.png"))
	{
		throw "Can't find icon";
	}

	if (!special_icon_tex.loadFromFile("Sprites/sparkles.png"))
	{
		throw "Can't find icon";
	}

	if (!wave_tex.loadFromFile("Sprites/waves.png"))
	{
		throw "Can't find icon";
	}

	if (!entity_font.openFromFile("Cinzel-Bold.ttf"))
	{
		throw "Can't find entity text";
	}
}

Level::~Level()
{
}

// Load a new level
void Level::LoadLevel()
{
	// Open the file from this path
	std::fstream load_file_stream;
	load_file_stream.open(filepath, std::ios::in);

	std::string load_file_string;
	int line_count = 0;

	// Load file to 2D Array
	if (load_file_stream.is_open())
	{
		while (std::getline(load_file_stream, load_file_string))
		{
			for (int i = 0; i < load_file_string.size(); i++)
			{
				level_array[i][line_count] = load_file_string[i];
			}
			line_count++;
		}

		load_file_stream.close();
	}

	// collumns
	for (int y = 0; y < level_height; y++)
	{
		// rows
		for (int x = 0; x < level_width; x++)
		{
			// Wave
			if (level_array[x][y] == 'w')
			{
				// Spawn a box at current location
				sf::RectangleShape* NewBox = new sf::RectangleShape({ 64, 64 });
				NewBox->setPosition(sf::Vector2f(x * 64, y * 64));
				NewBox->setTexture(&wave_tex);
				wall_tiles.push_back(NewBox);
			}

			// Default tile
			else
			{
				// Spawn a box at current location
				sf::RectangleShape* NewBox = new sf::RectangleShape({ 64, 64 });
				NewBox->setPosition(sf::Vector2f(x * 64, y * 64));
				NewBox->setOutlineThickness(-2.f);
				NewBox->setOutlineColor(sf::Color::Black);
				level_back_tiles.push_back(NewBox);
			}

			// Entities
			// Melee Enemy
			if (level_array[x][y] == 'a')
			{
				sf::Vector2f start_pos(x * 64, y * 64);
				Enemy NewEnemy(start_pos, melee);
				NewEnemy.entity_text.setFont(entity_font);
				enemies.push_back(NewEnemy);
			}

			// Ranged enemy
			if (level_array[x][y] == 'b')
			{
				sf::Vector2f start_pos(x * 64, y * 64);
				Enemy NewEnemy(start_pos, ranged);
				NewEnemy.entity_text.setFont(entity_font);
				enemies.push_back(NewEnemy);
			}

			// Tank enemy
			if (level_array[x][y] == 'c')
			{
				sf::Vector2f start_pos(x * 64, y * 64);
				Enemy NewEnemy(start_pos, tank);
				NewEnemy.entity_text.setFont(entity_font);
				enemies.push_back(NewEnemy);
			}

			// Melee Player
			if (level_array[x][y] == 'm')
			{
				sf::Vector2f start_pos(x * 64, y * 64);
				Player NewPlayer(start_pos, melee);

				NewPlayer.player_shape.setTexture(&player_tex);
				NewPlayer.attack_icon.setTexture(&attack_icon_tex);
				NewPlayer.move_icon.setTexture(&footstep_icon_tex);
				NewPlayer.special_icon.setTexture(&special_icon_tex);
				NewPlayer.entity_text.setFont(entity_font);

				players.push_back(NewPlayer);
			}

			// Ranged Player
			if (level_array[x][y] == 'r')
			{
				sf::Vector2f start_pos(x * 64, y * 64);
				Player NewPlayer(start_pos, ranged);

				NewPlayer.player_shape.setTexture(&player_tex);
				NewPlayer.attack_icon.setTexture(&attack_icon_tex);
				NewPlayer.move_icon.setTexture(&footstep_icon_tex);
				NewPlayer.special_icon.setTexture(&special_icon_tex);
				NewPlayer.entity_text.setFont(entity_font);

				players.push_back(NewPlayer);
			}

			// Tank Player
			if (level_array[x][y] == 't')
			{
				sf::Vector2f start_pos(x * 64, y * 64);
				Player NewPlayer(start_pos, tank);

				NewPlayer.player_shape.setTexture(&player_tex);
				NewPlayer.attack_icon.setTexture(&attack_icon_tex);
				NewPlayer.move_icon.setTexture(&footstep_icon_tex);
				NewPlayer.special_icon.setTexture(&special_icon_tex);
				NewPlayer.entity_text.setFont(entity_font);

				players.push_back(NewPlayer);
			}
		}
	}

	level_tiles.push_back(wall_tiles);
	level_tiles.push_back(level_back_tiles);

	if (players.size() == 0 || enemies.size() == 0)
	{
		throw "Level needs at least one player and enemy";
	}

	// Set the firset player and enemy as the current
	enemies[current_enemy].SetTarget();
	players[current_player].SetTarget();
}

// Unload this level
void Level::UnloadLevel()
{
	// Clear out all the current tiles in the level
	for (int i = 0; i < level_tiles.size(); i++)
	{
		level_tiles[i].clear();
	}

	level_tiles.clear();
	players.clear();
	enemies.clear();

	// Get the shortest amount of turns
	if (turns < best_turns)
	{
		best_turns = turns;
	}
}

// Reset the room
void Level::Reset()
{
	// Reset all the players
	for (int i = 0; i < players.size(); i++)
	{
		players[i].Reset();
	}

	// Reset all the enemies
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i].Reset();
	}

	turns = 0;
}

// Player clicks a position on the grid
void Level::Click(sf::Vector2f _mouse_pos)
{
	bool touching_enemy = false;

	for (int i = 0; i < enemies.size(); i++)
	{
		// Did user click on an enemy
		if (enemies[i].enemy_shape.getGlobalBounds().contains(_mouse_pos))
		{
			// Change selected enemy
			enemies[current_enemy].UnsetTarget();
			current_enemy = i;
			touching_enemy = true;
		}
	}

	for (int i = 0; i < players.size(); i++)
	{
		// Did user click on an player
		if (players[i].player_shape.getGlobalBounds().contains(_mouse_pos))
		{
			// Change selected player
			players[current_player].UnsetTarget();
			current_player = i;
		}
	}

	for (int i = 0; i < level_back_tiles.size(); i++)
	{
		// Did user click on an empty tile
		if (level_back_tiles[i]->getGlobalBounds().contains(_mouse_pos))
		{
			// Move the selected player if they selected an empty tile
			if (!touching_enemy)
			{
				players[current_player].MoveTo(_mouse_pos);
			}
		}
	}

	enemies[current_enemy].SetTarget();
	players[current_player].SetTarget();
}

// Display information to the user
void Level::Display()
{
	// Player events
	players[current_player].Animate();
	players[current_player].Display();
	players[current_player].UpdateIcons(); // Change colour of icons when they have performed their action

	// Enemy events
	enemies[current_enemy].Display();
}

// Check collision between entitys and tiles
void Level::CollisionChecks()
{
	int k = 0;
	// Check for collisions between player range tiles
	for (int i = 0; i < players[current_player].range_tiles.size(); i++)
	{
		// AND Wall tiles
		for (int j = 0; j < wall_tiles.size(); j++)
		{
			// This tile is colliding with this wall
			if (players[current_player].range_tiles[i].getGlobalBounds().findIntersection(wall_tiles[j]->getGlobalBounds()))
			{
				players[current_player].range_tiles[i].setFillColor(sf::Color(0, 0, 0, 0));
			}

			// This tile isn't colliding with any wall
			else if (k + 1 == wall_tiles.size())
			{
				players[current_player].range_tiles[i].setFillColor(sf::Color(0, 0, 255, 128)); // Light Blue
			}

			// Not colliding with this wall
			else
			{
				k++;
			}
		}
		k = 0;

		// Out of the map
		if (players[current_player].range_tiles[i].getPosition().x > 600.f || players[current_player].range_tiles[i].getPosition().y > 600.f)
		{
			players[current_player].range_tiles[i].setFillColor(sf::Color(0, 0, 0, 0)); // Range tiles no longer visible
		}
	}

	// Enemy colliding with
	for (int j = 0; j < enemies.size(); j++)
	{
		// Wall tiles
		for (int i = 0; i < wall_tiles.size(); i++)
		{
			if (enemies[j].enemy_shape.getGlobalBounds().findIntersection(wall_tiles[i]->getGlobalBounds()))
			{
				enemies[j].MoveBack();
			}
		}
		
		// Players
		for (int i = 0; i < players.size(); i++)
		{
			if (enemies[j].enemy_shape.getGlobalBounds().findIntersection(players[i].player_shape.getGlobalBounds()))
			{
				enemies[j].MoveBack();
			}
		}

		// With other enemies
		for (int i = 0; i < enemies.size(); i++)
		{
			// Don't compare with self
			if (i != j)
			{
				if (enemies[j].enemy_shape.getGlobalBounds().findIntersection(enemies[i].enemy_shape.getGlobalBounds()))
				{
					enemies[j].MoveBack();
				}
			}
		}
	}
}

// If false, player wins
bool Level::CheckAllEnemiesAlive()
{
	// Check to see if any enemies have health, if not proceed to next level
	int enemies_dead = 0;
	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemies[i].GetHealth() <= 0)
		{
			enemies_dead++;
		}

		// All enemies killed, move to next level
		if (enemies_dead == enemies.size())
		{
			return false;
		}
	}

	return true;
}

// If false, enemy wins
bool Level::CheckAllPlayersAlive()
{
	// Check to see if any enemies have health, if not proceed to next level
	int players_dead = 0;
	for (int i = 0; i < players.size(); i++)
	{
		if (players[i].GetHealth() <= 0)
		{
			players_dead++;
		}

		// All enemies killed, move to next level
		if (players_dead == players.size())
		{
			return false;
		}
	}

	return true;
}

// Selected player attacks selected enemy and vice versa
void Level::Combat()
{
	Attack(&players[current_player], &enemies[current_enemy]);
}

void Level::PlayerSpecial()
{
	players[current_player].Special();
}

// First is attacker and second is blocker
void Level::Attack(Entity* _attack, Entity* _defend)
{
	if (_attack->can_attack && _attack->GetHealth() > 0)
	{
		// Check the target is within range
		if (abs(_defend->GetShape()->getPosition().x - _attack->GetShape()->getPosition().x) < _attack->GetRange()*65.f && abs(_defend->GetShape()->getPosition().y - _attack->GetShape()->getPosition().y) < _attack->GetRange()*65.f)
		{

			int crit_hit = rand() % 100;

			// Critical hit
			if (crit_hit > _attack->GetCritChance())
			{
				_defend->SetHealth(_defend->GetHealth() - _attack->GetDamage() * 2); // Double damage
			}

			// Normal hit
			else
			{
				_defend->SetHealth(_defend->GetHealth() - _attack->GetDamage());
			}

			_attack->can_attack = false;
		}
	}
}

// Check if all player moves have been done
void Level::PassTurn()
{
	bool pass = true;

	for (int i = 0; i < players.size(); i++)
	{
		if (infinite_turn)
		{
			players[i].CanAction();
		}

		if (players[i].can_attack || players[i].can_move || players[i].can_special)
		{
			pass = false;
		}
	}

	// Automatically switch to the enemies turn
	if (pass)
	{
		EnemyTurn();
	}
}

// Perform all of the enemy moves and give the player back their turn
void Level::EnemyTurn()
{
	// Players need to be alive
	if (CheckAllPlayersAlive())
	{
		// Every enemy performs an action
		for (int i = 0; i < enemies.size(); i++)
		{
			// Check all players in range
			for (int j = 0; j < players.size(); j++)
			{
				Attack(&enemies[i], &players[j]); // If in range, attack first
				enemies[i].Move();
				Attack(&enemies[i], &players[j]); // Attack now, if now in range

				int special_action = rand() % 2;

				if (special_action == 1)
				{
					enemies[i].Special();
				}
			}

			enemies[i].CanAction();
		}

		turns++; // Next turn

		// Allow all player units to move again
		for (int i = 0; i < players.size(); i++)
		{
			players[i].CanAction();
		}
	}
}

// One function to call all other level functions
void Level::Update()
{
	CollisionChecks();
	PassTurn();
	Display();
}

// Print info to screen
void Level::Draw(sf::RenderWindow& _window)
{
	// Draw all level tiles
	for (int i = 0; i < level_tiles.size(); i++)
	{
		// Draw all specific tile type
		for (int j = 0; j < level_tiles[i].size(); j++)
		{
			_window.draw(*level_tiles[i][j]);
		}
	}

	// Draw the CURRENT player's range tiles
	for (int i = 0; i < players[current_player].range_tiles.size(); i++)
	{
		_window.draw(players[current_player].range_tiles[i]);
	}

	// Draw ALL the player units
	for (int i = 0; i < players.size(); i++)
	{
		_window.draw(players[i].player_shape);
	}

	// Draw current player's state
	sf::Text PlayerStatsText(entity_font, "Current Player Stats:", 30);
	PlayerStatsText.setPosition(sf::Vector2f(700, 0));
	_window.draw(PlayerStatsText);

	players[current_player].entity_text.setPosition(sf::Vector2f(700, 30));
	_window.draw(players[current_player].entity_text);

	_window.draw(players[current_player].attack_icon);
	_window.draw(players[current_player].move_icon);
	_window.draw(players[current_player].special_icon);

	// Draw ALL the enemies
	for (int i = 0; i < enemies.size(); i++)
	{
		_window.draw(enemies[i].enemy_shape);
	}

	// Draw current enemy's state
	sf::Text EnemyStatsText(entity_font, "Current Enemy Stats:", 30);
	EnemyStatsText.setPosition(sf::Vector2f(700, 150));
	_window.draw(EnemyStatsText);
	
	enemies[current_enemy].entity_text.setPosition(sf::Vector2f(700, 180));
	_window.draw(enemies[current_enemy].entity_text);

	// Draw the current turn
	sf::Text TurnText(entity_font, "Turns: " + std::to_string(turns), 30);
	TurnText.setPosition({ 150.f, 700.f });
	_window.draw(TurnText);

	// Draw if the player wins or loses
	sf::Text CompletionText(entity_font, "Players win", 30);
	CompletionText.setPosition({800.f, 400.f});

	if (!CheckAllEnemiesAlive())
	{
		CompletionText.setString("Players win");
	}

	if (!CheckAllPlayersAlive())
	{
		CompletionText.setString("Enemies win");
	}

	if (!CheckAllEnemiesAlive() || !CheckAllPlayersAlive())
	{
		_window.draw(CompletionText);
	}
}
