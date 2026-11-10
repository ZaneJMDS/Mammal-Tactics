#include "Player.h"

Player::Player(sf::Vector2f _start_pos, EntityTypes _entity_type)
{
	start_pos = _start_pos;
	entity_type = _entity_type;

	// y = (2x + 1)^2
	// Y is the number of tiles to be calculated and x is the range value

	player_shape.setSize({64.f, 64.f});

	SetStats();

	// Icon text location
	attack_icon.setSize({ 32.f, 32.f });
	move_icon.setSize({ 32.f, 32.f });
	special_icon.setSize({ 32.f, 32.f });
	attack_icon.setPosition({ 700.f, 0.f });
	move_icon.setPosition({ 700.f, 100.f });

	entity_text.setPosition({ 800, 0 });

	// Location and size
	player_shape.setTextureRect(sf::IntRect({ 0, 0 }, { 32, 32 }));

	Reset();

	// y = (2x + 1)^2
	// Y is the number of tiles to be calculated and x is the range value
	// Range tiles that move with the player
	for (int i = -range; i < range+1; i++)
	{
		for (int j = -range; j < range+1; j++)
		{
			// Maybe ignore middle tile
			if (i != 0 && j != 0);
			{
				sf::RectangleShape NewRangeTile;
				NewRangeTile.setSize({ 64.f, 64.f });
				NewRangeTile.setFillColor(sf::Color(0, 0, 255, 128)); // See through blue

				NewRangeTile.setPosition({ (float)(64 * i) + player_shape.getPosition().x, (float)(64 * j) + player_shape.getPosition().y });

				range_tiles.push_back(NewRangeTile);
			}
		}
	}
}

Player::~Player()
{
}

// Move to the mouse position
void Player::MoveTo(sf::Vector2f _move_pos)
{
	// Round it, so that move pos is a multiple of 64
	sf::Vector2f move_pos;
	move_pos.x = std::floor(_move_pos.x / 64) * 64;
	move_pos.y = std::floor(_move_pos.y / 64) * 64;

	// Check that the player is in range
	if (abs(move_pos.x - player_shape.getPosition().x) < range * 65 && abs(move_pos.y - player_shape.getPosition().y) < range * 65 && can_move && player_shape.getPosition() != move_pos)
	{
		player_shape.setPosition(move_pos);

		// Range tiles that move with the player
		int k = 0;
		for (int i = -range; i < range + 1; i++)
		{
			for (int j = -range; j < range + 1; j++)
			{
				range_tiles[k].setPosition({64.f * i + move_pos.x, 64.f * j + move_pos.y});
				k++;
			}
		}

		can_move = false;
	}
}

// Could do area damage, heal, or buff
void Player::Special()
{
	// Area
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1))
	{
		std::cout << "Area attack";
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

void Player::UpdateIcons()
{
	if (can_move)
	{
		move_icon.setFillColor(sf::Color::White);
	}

	else
	{
		move_icon.setFillColor(sf::Color::Black);
	}

	if (can_attack)
	{
		attack_icon.setFillColor(sf::Color::White);
	}

	else
	{
		attack_icon.setFillColor(sf::Color::Black);
	}
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