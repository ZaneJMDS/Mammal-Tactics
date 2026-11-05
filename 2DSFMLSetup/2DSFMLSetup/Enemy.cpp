#include "Enemy.h"

Enemy::Enemy(sf::Vector2f _start_pos)
{
	enemy_shape.setSize({ 32.f, 32.f });
	enemy_shape.setFillColor(sf::Color::Green);
	start_pos = _start_pos;

	Reset();

	srand(time(0)); 
}

Enemy::~Enemy()
{
}

// Move the enemy in a random direction
void Enemy::Move()
{
	int hor_input = rand() % 3; // Decides the direction from eight different options
	int ver_input = rand() % 3;
	float horizontal_movement[3] = { -64.f, 0, 64.f };
	float vertical_movement[3] = { -64.f, 0, 64.f };

	// Check not out of bounds
	if (enemy_shape.getPosition().x + horizontal_movement[hor_input] < 550 && enemy_shape.getPosition().x + horizontal_movement[hor_input] > 40 && enemy_shape.getPosition().y + vertical_movement[ver_input] > 40 && enemy_shape.getPosition().y + vertical_movement[ver_input] < 550)
	{
		enemy_shape.move({ horizontal_movement[hor_input], vertical_movement[ver_input] });
	}

	else
	{
		Move();
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
	enemy_shape.setPosition(start_pos);
	UnsetTarget(); // Reset level so player chooses a new target
}

// Display the enemies stats
void Enemy::Display()
{

}
