/***********************************************************************
Author      :	Zane Jackson
Mail        :   Zane.Jackson@mds.ac.nz
Description :	Managing, loading, and unloading levels
File name   :   Level.h
**************************************************************************/

#pragma once
#include "Player.h"
#include "Enemy.h"

#include <fstream>

class Level
{
public:
	// Level dimensions
	static const int level_width = 10;
	static const int level_height = 10;

	char level_array[level_width][level_height]; // Array holding level tiles

	// Static blocks
	std::vector<sf::RectangleShape*> level_back_tiles;
	std::vector<sf::RectangleShape*> wall_tiles;

	std::vector<std::vector<sf::RectangleShape*>> level_tiles; // Remember to add new tiles to LoadLevel

	// Entities
	sf::Texture player_tex;
	sf::Texture attack_icon_tex;
	sf::Texture footstep_icon_tex;
	sf::Texture special_icon_tex;
	sf::Texture wave_tex;
	sf::Font entity_font;

	std::vector<Enemy> enemies;
	std::vector<Player> players;

	Level(std::string _filepath);
	~Level();

	void LoadLevel();
	void UnloadLevel();
	void Reset();

	void Click(sf::Vector2f _mouse_pos);
	void Display();
	void CollisionChecks();
	
	bool CheckAllEnemiesAlive();
	bool CheckAllPlayersAlive();
	
	void Combat();
	void PlayerSpecial();
	void Attack(Entity* _attack, Entity* _defend);
	
	void PassTurn();
	void EnemyTurn();
	int GetTurn() { return turns; }

	void Update();
	void Draw(sf::RenderWindow& _window);

	void ToggleInfiniteTurn() { infinite_turn = !infinite_turn; }
	bool infinite_turn = false;

private:
	std::string filepath;
	int turns = 0;
	int best_turns = 100;
	int current_player = 0;
	int current_enemy = 0;
};

