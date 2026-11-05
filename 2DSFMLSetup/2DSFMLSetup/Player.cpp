#include "Player.h"

Player::Player(sf::Vector2f _start_pos) : PlayerFont("Cinzel-Bold.ttf"), PlayerText(PlayerFont, "", 16)
{
	start_pos = _start_pos;

	player_shape.setSize({32.f, 32.f});

	// Location and size
	PlayerText.setPosition({300, 700});
	player_shape.setTextureRect(sf::IntRect({ 0, 0 }, { 32, 32 }));

	Reset();

	// Range tiles that move with the player
	for (int i = 0; i < 9; i++)
	{
		sf::RectangleShape NewRangeTile;
		NewRangeTile.setSize({64.f, 64.f});
		NewRangeTile.setFillColor(sf::Color(0, 0, 255, 128)); // See through blue

		// First collumn
		if (i < 3)
		{
			NewRangeTile.setPosition({0.f, (float)(64*i)});
		}

		// Second collumn
		else if (i < 6)
		{
			NewRangeTile.setPosition({64.f, (float)(64*(i-3))});
		}

		// Third collumn
		else
		{
			NewRangeTile.setPosition({ 128.f, (float)(64 * (i - 6)) });
		}

		range_tiles.push_back(NewRangeTile);
	}
}

Player::~Player()
{
}

// Move the player one tile in direction held
void Player::Move(Enemy* _Enemy)
{
	// Left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		// Check not out of bounds
		if (player_shape.getPosition().x > 40)
		{
			player_shape.move({ -64.f, 0.f });

			for (int i = 0; i < 9; i++)
			{
				range_tiles[i].move({ -64.f, 0.f });
			}

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

			for (int i = 0; i < 9; i++)
			{
				range_tiles[i].move({ 64.f, 0.f });
			}

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

			for (int i = 0; i < 9; i++)
			{
				range_tiles[i].move({ 0.f, -64.f });
			}

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

			for (int i = 0; i < 9; i++)
			{
				range_tiles[i].move({ 0.f, 64.f });
			}

			_Enemy->Move();
		}
	}
}

void Player::MoveTo(sf::Vector2f _move_pos)
{
	sf::Vector2f move_pos;
	move_pos.x = std::floor(_move_pos.x / 64) * 64;
	move_pos.y = std::floor(_move_pos.y / 64) * 64;

	// Check that the player is in range
	if (abs(move_pos.x - player_shape.getPosition().x) < range && abs(move_pos.y - player_shape.getPosition().y) < range)
	{
		player_shape.setPosition(move_pos);

		// First collumn
		// Range tiles that move with the player
		for (int i = 0; i < 9; i++)
		{
			if (i < 3)
			{
				range_tiles[i].setPosition({ -64.f + move_pos.x, (float)(64 * i) - 64.f + move_pos.y });
			}

			// Second collumn
			else if (i < 6)
			{
				range_tiles[i].setPosition({ 0.f + move_pos.x, (float)(64 * (i - 3)) - 64.f + move_pos.y });
			}

			// Third collumn
			else
			{
				range_tiles[i].setPosition({ 64.f + move_pos.x, (float)(64 * (i - 6)) - 64.f + move_pos.y });
			}
		}
	}
}

// Player has selected an enemy and attacks them
void Player::Attack(Enemy* _Enemy)
{
	// Print out ranges
	std::cout << "Range difference x: " << abs(_Enemy->enemy_shape.getPosition().x - player_shape.getPosition().x) << "\n";
	std::cout << "Range difference y: " << abs(_Enemy->enemy_shape.getPosition().y - player_shape.getPosition().y) << "\n";

	// Check the enemy is in range on the x and y axis
	if (abs(_Enemy->enemy_shape.getPosition().x - player_shape.getPosition().x) < range && abs(_Enemy->enemy_shape.getPosition().y - player_shape.getPosition().y) < range)
	{
		_Enemy->SetHealth(_Enemy->GetHealth() - damage);
	}

	// Enemy is not in range
	else
	{
		std::cout << "Enemy is not in range\n";
	}
}

// Could do area damage, heal, or buff
void Player::Special(Enemy* _Enemy)
{
	// Area
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1))
	{
		std::cout << "Area attack";

		// Check the enemy is in range
		if (abs(_Enemy->enemy_shape.getPosition().x - player_shape.getPosition().x) < range && abs(_Enemy->enemy_shape.getPosition().y - player_shape.getPosition().y) < range)
		{
			_Enemy->SetHealth(_Enemy->GetHealth() - damage);
		}
	}

	// Heal
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2))
	{
		if (health < max_health)
		{
			std::cout << "Healing...";
			health += 2;
		}
	}

	// Damage up
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3))
	{
		std::cout << "Damage up";
		damage++;
	}
}

// Get the stats comunicates to the player
void Player::Display()
{
	PlayerText.setString(
		"Player Stats: "
		"Health: " + std::to_string(health) +
		"Damage: " + std::to_string(damage) +
		"Range: " + std::to_string(damage)
	);
	
}

// Animate the player's frames
void Player::Animate()
{
	// How many seconds have passed
	sf::Time elapsed2 = animation_clock.getElapsedTime();
	float current_time = (elapsed2.asSeconds());

	// FRAMES
	if (current_time < 0.5f) { player_shape.setTextureRect(sf::IntRect({ 0, 0 }, { 32, 32 })); } // Location and size of texture
	else if (current_time < 1.f) { player_shape.setTextureRect(sf::IntRect({ 32, 0 }, { 32, 32 })); }
	else if (current_time < 1.5f) { player_shape.setTextureRect(sf::IntRect({ 64, 0 }, { 32, 32 })); }
	else if (current_time < 2.f) { player_shape.setTextureRect(sf::IntRect({ 96, 0 }, { 32, 32 })); }
	else { animation_clock.restart(); }
}

// Bring player back to the starting square when they die
void Player::Reset()
{
	player_shape.setPosition({ 64.f, 64.f });
}
