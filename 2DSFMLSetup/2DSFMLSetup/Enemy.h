#pragma once
#include <SFML/Graphics.hpp>
#include <Random>

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

private:
	int health = 10;
	int damage = 3;
};

