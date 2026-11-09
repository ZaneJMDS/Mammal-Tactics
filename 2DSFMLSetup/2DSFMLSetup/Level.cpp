#include "level.h"

Level::Level(std::string _filepath) : filepath(_filepath)
{
	if (!player_tex.loadFromFile("Sprites/player.png"))
	{
		throw "Can't find player texture";
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
			// Wall
			if (level_array[x][y] == 'w')
			{
				// Spawn a box at current location
				sf::RectangleShape* NewBox = new sf::RectangleShape({ 64, 64 });
				NewBox->setPosition(sf::Vector2f(x * 64, y * 64));
				NewBox->setFillColor(sf::Color::Black);
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
			// Default Enemy
			if (level_array[x][y] == 'e')
			{
				sf::Vector2f start_pos(x * 64, y * 64);
				Enemy NewEnemy(start_pos);
				NewEnemy.entity_text.setFont(entity_font);
				enemies.push_back(NewEnemy);
			}

			// Player
			if (level_array[x][y] == 'p')
			{
				sf::Vector2f start_pos(x * 64, y * 64);
				Player NewPlayer(start_pos);

				// Why do I have to hold these here?
				NewPlayer.player_shape.setTexture(&player_tex);
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
}

// Check info or move the player
void Level::Click(sf::Vector2f _mouse_pos)
{
	for (int i = 0; i < enemies.size(); i++)
	{
		// Did user click on an enemy
		if (enemies[i].enemy_shape.getGlobalBounds().contains(_mouse_pos))
		{
			// Change selected enemy
			enemies[current_enemy].UnsetTarget();
			current_enemy = i;
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
			// Move the selected player
			players[current_player].MoveTo(_mouse_pos);
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
	players[current_player].Display(); // Communicate the top is player

	// Enemy events
	enemies[current_enemy].Animate();
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
	}

	// Wall tiles
	for (int i = 0; i < wall_tiles.size(); i++)
	{
		// With player
		if (players[current_player].player_shape.getGlobalBounds().findIntersection(wall_tiles[i]->getGlobalBounds()))
		{
			players[current_player].player_shape.move({ -64, 0 });
		}

		// With enemy
		for (int j = 0; j < enemies.size(); j++)
		{
			if (enemies[j].enemy_shape.getGlobalBounds().findIntersection(wall_tiles[i]->getGlobalBounds()))
			{
				enemies[j].enemy_shape.move({ -64, 0 }); // TODO NEED TO IMPROVE
			}
		}
	}
}

// Selected player attacks selected enemy and vice versa
void Level::Combat()
{
	if (players[current_player].can_attack)
	{
		Attack(&players[current_player], &enemies[current_enemy]);
	}


	Attack(&enemies[current_enemy], &players[current_player]);
}

// First is attacker and second is blocker
void Level::Attack(Entity* _attack, Entity* _defend)
{
	// Check the target is within range
	if (abs(_defend->GetShape().getPosition().x - _attack->GetShape().getPosition().x < _attack->GetRange() && _defend->GetShape().getPosition().y - _attack->GetShape().getPosition().y < _attack->GetRange()))
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
	}

	else
	{
		std::cout << "Enemy is not in range\n";
	}
}

// Perform all of the enemy moves
void Level::EnemyTurn()
{
	Attack(&enemies[current_enemy], &players[current_player]); // If in range, attack first

	// Move all the enemies
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i].Move();
	}

	Attack(&enemies[current_enemy], &players[current_player]); // Attack now, if now in range

	turns++; // Next turn

	// Allow all player units to move again
	for (int i = 0; i < players.size(); i++)
	{
		players[i].can_move = true;
	}
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

	_window.draw(players[current_player].entity_text);

	// Draw ALL the enemies
	for (int i = 0; i < enemies.size(); i++)
	{
		_window.draw(enemies[i].enemy_shape);
	}

	_window.draw(enemies[current_enemy].entity_text);
}
