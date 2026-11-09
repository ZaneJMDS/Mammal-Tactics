#pragma once
#include <SFML/Graphics.hpp>
#include <Random>
#include <iostream>

#include "Entity.h"

class Enemy : public Entity
{
public:
	// Texture stored in level file
	sf::RectangleShape enemy_shape;

	Enemy(sf::Vector2f _start_pos, EntityTypes _entity_type);
	~Enemy();

	// Actions the enemy can take on their turn
	void Move();
	void Special();

	// Indicators for if the user has this enemy selected
	void SetTarget() override { enemy_shape.setFillColor(sf::Color::Blue); }
	void UnsetTarget() override { enemy_shape.setFillColor(sf::Color::Green); }

	void Animate();

	sf::RectangleShape GetShape() override { return enemy_shape; }
};

