#include "level.h"

Level::Level(std::string _filepath) : filepath(_filepath)
{
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
		// collumns
		while (std::getline(load_file_stream, load_file_string))
		{
			// rows
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
			// Default tile
			// Spawn a box at current location
			sf::RectangleShape* NewBox = new sf::RectangleShape({ 64, 64 });
			NewBox->setPosition(sf::Vector2f(x * 64, y * 64));
			NewBox->setOutlineThickness(-2.f);
			NewBox->setOutlineColor(sf::Color::Black);
			level_back_tiles.push_back(NewBox);
		}
	}

	level_tiles.push_back(level_back_tiles);
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
void Level::Reset(Player* _player)
{
	_player->Reset();
}
