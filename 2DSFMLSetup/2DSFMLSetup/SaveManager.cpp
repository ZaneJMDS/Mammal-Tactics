#include "SaveManager.h"

SaveManager::SaveManager()
{
}

SaveManager::~SaveManager()
{
}

// Save current level to file
void SaveManager::SaveFile(int _current_level, bool _vsync, sf::Vector2u _resolution, std::vector<int> _level_turns)
{
	// Open the file from this path
	std::ofstream write_file;
	write_file.open("Save1.txt");

	// Write levels beaten
	write_file.clear();
	write_file << _current_level << "\n" << _vsync << "\n" << _resolution.x << ',' << _resolution.y << "\n";

	// Print out level_turns
	for (int i = 0; i < _level_turns.size(); i++)
	{
		write_file << _level_turns[i] << ',';
	}

	write_file.close();
}

// Open current level from file
void SaveManager::LoadFile()
{
	// Open the file from this path
	std::fstream load_file_stream;
	load_file_stream.open("Save1.txt", std::ios::in);
	std::string load_file_string;

	// Get Levels beaten
	std::getline(load_file_stream, load_file_string);
	completed_levels = (stoi(load_file_string));

	// Get Vsync state
	std::getline(load_file_stream, load_file_string);
	Vsync = (stoi(load_file_string));

	// Get the resolution
	std::getline(load_file_stream, load_file_string, ',');
	resolution.x = stoi(load_file_string);
	
	if (resolution.x < 400) { resolution.x = 400; }

	std::getline(load_file_stream, load_file_string);
	resolution.y = stoi(load_file_string);

	if (resolution.y < 400) { resolution.y = 400; }

	// Get the lowest amount of turns for each level
	for (int i = 0; i < 3; i++)
	{
		std::getline(load_file_stream, load_file_string, ',');
		level_turns.push_back(stoi(load_file_string));
	}

	load_file_stream.close();
}

// Revert all progress in this save file
void SaveManager::ClearSave()
{
	// Open the file from this path
	std::ofstream write_file;
	write_file.open("Save1.txt");

	// Revert progress back to zero
	// Write levels beaten
	write_file.clear();
	write_file << 0 << "\n" << 1 << "\n" << resolution.x << ',' << resolution.y << "\n";

	// Print out level_turns
	for (int i = 0; i < level_turns.size(); i++)
	{
		write_file << 100 << ',';
	}

	write_file.close();
}
