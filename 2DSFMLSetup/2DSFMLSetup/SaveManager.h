#pragma once
#include <fstream>
#include <string>

class SaveManager
{
public:
	SaveManager();
	~SaveManager();

	void SaveFile(int _current_level);
	int LoadFile();
	void ClearSave();
};

