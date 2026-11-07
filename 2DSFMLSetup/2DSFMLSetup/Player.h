/***********************************************************************
Author      :	Zane Jackson
Mail        :   Zane.Jackson@mds.ac.nz
Description :	Class for managing the player object
File name   :   Player.h
**************************************************************************/

#include <SFML/Graphics.hpp>
#include <iostream>
#pragma once
#include "Entity.h"

class Player : public Entity
{
public:
	// The player shape should remain
	sf::RectangleShape player_shape;
	std::vector<sf::RectangleShape> range_tiles;

	Player(sf::Vector2f _start_pos);
	~Player();

	// Actions the player can take on their turn
	void Move();
	void MoveTo(sf::Vector2f _move_pos);
	void Attack();
	void Special();

	// Indicators for if the user has this enemy selected
	void SetTarget() { player_shape.setFillColor(sf::Color::Blue); }
	void UnsetTarget() { player_shape.setFillColor(sf::Color::Green); }

	void Animate();
	void Reset();

	sf::RectangleShape GetShape() override { return player_shape; }

	bool can_move = true; // Player always gets the first turn

private:
	// Stats
	const int max_health = 20;
	sf::Vector2f start_pos;

	sf::Clock animation_clock;
};

