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

	Player();
	~Player();

	// Actions the player can take on their turn
	void Move(Enemy* _Enemy);
	void Attack();
	void Special();

	void Animate();
	void Reset();

private:
	int health = 10;
	int damage = 3;
};

