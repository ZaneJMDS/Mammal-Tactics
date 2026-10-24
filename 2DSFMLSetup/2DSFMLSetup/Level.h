/***********************************************************************
Author      :	Zane Jackson
Mail        :   Zane.Jackson@mds.ac.nz
Description :	Managing, loading, and unloading levels
File name   :   Level.h
**************************************************************************/

#pragma once
#include "Player.h"
#include <fstream>

class Level
{
public:
	// Level dimensions
	static const int level_width = 10;
	static const int level_height = 10;

	char level_array[level_width][level_height]; // Array holding level tiles

	std::vector<sf::RectangleShape*> level_back_tiles;

	std::vector<std::vector<sf::RectangleShape*>> level_tiles; // Remember to add new tiles to LoadLevel

	Level(std::string _filepath);
	~Level();

	void LoadLevel();
	void UnloadLevel();
	void Reset(Player* _player);

private:
	std::string filepath;
};

