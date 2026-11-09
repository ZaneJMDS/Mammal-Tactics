#include "Enemy.h"

Enemy::Enemy(sf::Vector2f _start_pos)
{
	enemy_shape.setSize({ 64.f, 64.f });
	enemy_shape.setFillColor(sf::Color::Green);
	start_pos = _start_pos;

	entity_text.setPosition({ 800, 200 });

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

	// Need to move
	if (hor_input == 1 && ver_input == 1)
	{
		Move();
	}

	if (health > 0)
	{
		// Check not out of bounds
		if (enemy_shape.getPosition().x + horizontal_movement[hor_input] < 550 && enemy_shape.getPosition().x + horizontal_movement[hor_input] > 40 && enemy_shape.getPosition().y + vertical_movement[ver_input] > 40 && enemy_shape.getPosition().y + vertical_movement[ver_input] < 550)
		{
			enemy_shape.move({ horizontal_movement[hor_input], vertical_movement[ver_input] });
		}

		else
		{
			Move();
		}
	}
}

void Enemy::Special()
{
	std::cout << "SPECIAL";
}

void Enemy::Animate()
{
}

// Reset the enemy to default state
void Enemy::Reset()
{
	enemy_shape.setPosition(start_pos);
	health = max_health;
	can_attack = true;
	UnsetTarget(); // Reset level so player chooses a new target
}
