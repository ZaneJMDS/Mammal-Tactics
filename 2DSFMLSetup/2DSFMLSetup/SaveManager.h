#pragma once
#include <fstream>
#include <string>

class SaveManager
{
public:
	SaveManager();
	~SaveManager();

	void SaveFile();
	void LoadFile();
	void ClearSave();

	int levels_beaten = 0;
};

