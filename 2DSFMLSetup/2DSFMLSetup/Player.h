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
	
	sf::RectangleShape move_icon;
	sf::RectangleShape attack_icon;
	sf::RectangleShape special_icon;

	Player(sf::Vector2f _start_pos, EntityTypes _entity_type);
	~Player();

	// Actions the player can take on their turn
	void Move();
	void MoveTo(sf::Vector2f _move_pos);
	void Special();

	// Indicators for if the user has this enemy selected
	void SetTarget() override { player_shape.setFillColor(sf::Color::Blue); }
	void UnsetTarget() override { player_shape.setFillColor(sf::Color::Green); }

	void Animate();

	sf::RectangleShape GetShape() override { return player_shape; }

private:
	sf::Clock animation_clock;
};

