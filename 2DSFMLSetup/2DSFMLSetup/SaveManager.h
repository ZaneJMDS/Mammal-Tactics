/***********************************************************************
Author      :	Zane Jackson
Description :	Save and load progress / selected settings
File name   :   SaveManager.h
**************************************************************************/

#pragma once
#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>

class SaveManager
{
public:
	SaveManager();
	~SaveManager();

	void SaveFile(int _current_level, bool _vsync, sf::Vector2u _resolution, std::vector<int> _level_turns);
	void LoadFile();
	void ClearSave();

	sf::Vector2u GetResolution() { return resolution; }
	bool GetVsync() { return Vsync; }
	int GetCurrentLevel() { return completed_levels; }
	std::vector<int> GetLevelTurns() { return level_turns; }

private:
	sf::Vector2u resolution;
	bool Vsync;
	int completed_levels;
	std::vector<int> level_turns;
};

