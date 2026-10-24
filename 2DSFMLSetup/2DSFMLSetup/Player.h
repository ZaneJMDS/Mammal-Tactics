/***********************************************************************
Author      :	Zane Jackson
Mail        :   Zane.Jackson@mds.ac.nz
Description :	Class for managing the player object
File name   :   Player.h
**************************************************************************/

#include <SFML/Graphics.hpp>
#pragma once

class Player
{
public:
	// The player shape and texture should remain
	sf::RectangleShape player_shape;

	Player();
	~Player();

	// Actions the player can take on their turn
	void Move();
	void Attack();
	void Special();

	void Animate();
	void Reset();

private:
	int health = 10;
	int damage = 3;
};

