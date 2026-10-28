#pragma once
#include <SFML/Graphics.hpp>
#include <Random>
#include <iostream>

class Enemy
{
public:
	// The player shape and texture should remain
	sf::RectangleShape enemy_shape;

	Enemy();
	~Enemy();

	// Actions the player can take on their turn
	void Move();
	void Attack();
	void Special();

	void Animate();
	void Reset();

	int GetHealth() { return health; }
	void SetHealth(int _health) { health = _health; }
	int GetDamage() { return damage; }

private:
	int health = 10;
	int damage = 3;
};

