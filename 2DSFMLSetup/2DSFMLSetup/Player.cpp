#include "Player.h"

Player::Player()
{
	player_shape.setSize({32.f, 32.f});
	player_shape.setFillColor(sf::Color::Black);
}

Player::~Player()
{
}

void Player::Move(Enemy* _Enemy)
{
	// Left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		// Check not out of bounds
		if (player_shape.getPosition().x > 40)
		{
			player_shape.move({ -64.f, 0.f });
			_Enemy->Move();
		}
	}

	// Right
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		// Check not out of bounds
		if (player_shape.getPosition().x < 550)
		{
			player_shape.move({ 64.f, 0.f });
			_Enemy->Move();
		}
	}

	// Up
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
	{
		// Check not out of bounds
		if (player_shape.getPosition().y > 40)
		{
			player_shape.move({ 0.f, -64.f });
			_Enemy->Move();
		}
	}

	// Down
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
	{
		// Check not out of bounds
		if (player_shape.getPosition().y < 550)
		{
			player_shape.move({ 0.f, 64.f });
			_Enemy->Move();
		}
	}
}

void Player::Attack()
{
}

void Player::Special()
{
}

void Player::Animate()
{
}

void Player::Reset()
{
}
