/***********************************************************************
Author      :	Zane Jackson
Mail        :   Zane.Jackson@mds.ac.nz
Description :	Class for managing the player object
File name   :   Player.h
**************************************************************************/

#include <SFML/Graphics.hpp>
#include "Enemy.h" // Player moves when enmy moves for now
#pragma once

class Player
{
public:
	// The player shape and texture should remain
	sf::RectangleShape player_shape;
	std::vector<sf::RectangleShape> range_tiles;
	sf::Font PlayerFont; // Font declared before the text
	sf::Text PlayerText;

	Player(sf::Vector2f _start_pos);
	~Player();

	// Actions the player can take on their turn
	void Move(Enemy* _Enemy);
	void MoveTo(sf::Vector2f _move_pos);
	void Attack(Enemy* _Enemy);
	void Special(Enemy* _Enemy);

	void Display();

	void Animate();
	void Reset();

	int GetHealth() { return health; }
	int GetDamage() { return damage; }

private:
	// Stats
	int health = 10;
	const int max_health = 20;
	int damage = 3;
	float range = 64.f + 1.f; // Each tile is 64 by 64px
	sf::Vector2f start_pos;

	sf::Clock animation_clock;
};

