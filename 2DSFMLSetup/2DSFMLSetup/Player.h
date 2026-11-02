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
	sf::Texture texture;
	std::vector<sf::RectangleShape> range_tiles;

	Player();
	~Player();

	// Actions the player can take on their turn
	void Move(Enemy* _Enemy);
	void Attack(Enemy* _Enemy);
	void Special(Enemy* _Enemy);

	void Animate();
	void Reset();

	int GetHealth() { return health; }
	int GetDamage() { return damage; }

private:
	int health = 10;
	const int max_health = 20;
	int damage = 3;
	float range = 64.f + 1.f; // Each tile is 64 by 64px
	sf::Clock animation_clock;
};

