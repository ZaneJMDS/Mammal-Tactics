#pragma once
#include <SFML/Graphics.hpp>
#include <Random>
#include <iostream>

class Enemy
{
public:
	// The player shape and texture should remain
	sf::RectangleShape enemy_shape;

	Enemy(sf::Vector2f _start_pos);
	~Enemy();

	// Actions the enemy can take on their turn
	void Move();
	void Attack();
	void Special();

	// Indicators for if the player has this enemy selected
	void SetTarget() { enemy_shape.setFillColor(sf::Color::Blue); }
	void UnsetTarget() { enemy_shape.setFillColor(sf::Color::Green); }

	void Animate();
	void Reset();

	void Display();

	int GetHealth() { return health; }
	void SetHealth(int _health) { health = _health; }
	int GetDamage() { return damage; }

private:
	sf::Vector2f start_pos;
	int health = 10;
	const int max_health = 20;
	int damage = 3;
	float range = 64.f + 1.f; // Each tile is 64 by 64px
};

