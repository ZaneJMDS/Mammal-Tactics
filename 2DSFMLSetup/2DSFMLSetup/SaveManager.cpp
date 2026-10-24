#include "SaveManager.h"

SaveManager::SaveManager()
{
}

SaveManager::~SaveManager()
{
}

void SaveManager::SaveFile()
{
	// Open the file from this path
	std::ofstream write_file;
	write_file.open("Save1.txt");

	// Write levels beaten
	write_file << levels_beaten;
	write_file.close();
}

void SaveManager::LoadFile()
{
	// Open the file from this path
	std::fstream load_file_stream;
	load_file_stream.open("Save1.txt", std::ios::in);
	std::string load_file_string;

	// Get Levels beaten
	std::getline(load_file_stream, load_file_string);
	levels_beaten = stoi(load_file_string);
	load_file_stream.close();
}

// Revert all progress in this save file
void SaveManager::ClearSave()
{
	levels_beaten = 0;

	// Open the file from this path
	std::ofstream write_file;
	write_file.open("Save1.txt");

	// Revert progress back to zero
	// Write levels beaten
	write_file << levels_beaten;
	write_file.close();
}
