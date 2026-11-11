#include "SaveManager.h"

SaveManager::SaveManager()
{
}

SaveManager::~SaveManager()
{
}

// Save current level to file
void SaveManager::SaveFile(int _current_level)
{
	// Open the file from this path
	std::ofstream write_file;
	write_file.open("Save1.txt");

	// Write levels beaten
	write_file.clear();
	write_file << _current_level;
	write_file.close();
}

// Open current level from file
int SaveManager::LoadFile()
{
	// Open the file from this path
	std::fstream load_file_stream;
	load_file_stream.open("Save1.txt", std::ios::in);
	std::string load_file_string;

	// Get Levels beaten
	std::getline(load_file_stream, load_file_string);
	int levels_beaten = stoi(load_file_string);
	load_file_stream.close();

	return levels_beaten;
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
	write_file << 0;
	write_file.close();
}
