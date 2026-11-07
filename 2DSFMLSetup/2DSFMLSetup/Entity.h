#pragma once
#include <string>
#include <SFML/Graphics.hpp>

class Entity
{
public:
	sf::Font no_font;
	sf::Text entity_text;

	Entity();
	~Entity();

	void Display();

	int GetHealth() { return health; }
	void SetHealth(int _health) { health = _health; }
	int GetDamage() { return damage; }
	float GetRange() { return range; }
	int GetCritChance() { return crit_chance; }
	virtual sf::RectangleShape GetShape() = 0;

protected:
	int health = 0;
	int damage = 3;
	float range = 64.f + 1.f; // Each tile is 64 by 64px
	int crit_chance = 50;
};

