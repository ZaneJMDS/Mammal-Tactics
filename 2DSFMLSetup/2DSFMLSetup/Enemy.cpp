#include "Enemy.h"

Enemy::Enemy()
{
	enemy_shape.setSize({ 32.f, 32.f });
	enemy_shape.setFillColor(sf::Color::Green);
	
	Reset();

	srand(time(0)); 
}

Enemy::~Enemy()
{
}

void Enemy::Move()
{
	int move_input = rand() % 4;

	// Left
	// Check not out of bounds
	if (move_input == 0)
	{
		if (enemy_shape.getPosition().x > 40)
		{
			enemy_shape.move({ -64.f, 0.f });
		}

		else { Move(); }
	}

	// Right
	if (move_input == 1)
	{
		if (enemy_shape.getPosition().x < 550)
		{
			enemy_shape.move({ 64.f, 0.f });
		}

		else { Move(); }
	}

	// Up
	if (move_input == 2)
	{
		if (enemy_shape.getPosition().y > 40)
		{
			enemy_shape.move({0.f, -64.f});
		}

		else { Move(); }
	}

	// Down
	if (move_input == 3)
	{
		if (enemy_shape.getPosition().y < 550)
		{
			enemy_shape.move({ 0.f, 64.f });
		}

		else { Move(); }
	}

	// Kill the enemy
	if (health < 0)
	{
		std::cout << "I am dead";
	}
}

void Enemy::Attack()
{
}

void Enemy::Special()
{
}

void Enemy::Animate()
{
}

void Enemy::Reset()
{
	enemy_shape.setPosition({ 64.f, 64.f });
}
