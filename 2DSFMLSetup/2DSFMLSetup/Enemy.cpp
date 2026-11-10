#include "Enemy.h"

Enemy::Enemy(sf::Vector2f _start_pos, EntityTypes _entity_type)
{
	enemy_shape.setSize({ 64.f, 64.f });
	enemy_shape.setFillColor(sf::Color::Green);
	
	start_pos = _start_pos;
	entity_type = _entity_type;

	SetStats();

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

	// Can't be center
	if (hor_input == 1 && ver_input == 1)
	{
		Move();
	}

	if (health > 0 && can_move)
	{
		// Check not out of bounds of the grid
		if (enemy_shape.getPosition().x + horizontal_movement[hor_input] < 550 && enemy_shape.getPosition().x + horizontal_movement[hor_input] > 40 && enemy_shape.getPosition().y + vertical_movement[ver_input] > 40 && enemy_shape.getPosition().y + vertical_movement[ver_input] < 550)
		{
			enemy_shape.move({ horizontal_movement[hor_input], vertical_movement[ver_input] });
			can_move = false;
		}

		// Not a valid move
		else
		{
			Move();
		}
	}
}

void Enemy::Special()
{
	switch (entity_type)
	{
	case super:
		damage *= 2;
		range += 1;
	}
}

void Enemy::Animate()
{
}
