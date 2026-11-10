/***********************************************************************
Author      :	Zane Jackson
Mail        :   Zane.Jackson@mds.ac.nz
Description :	Class for managing all entities
File name   :   Entity.h
**************************************************************************/

#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <iostream>

enum EntityTypes
{
	melee,
	ranged,
	tank,
	super
};

class Entity
{
public:
	sf::Font no_font;
	sf::Text entity_text;

	Entity();
	~Entity();

	virtual void SetTarget() = 0;
	virtual void UnsetTarget() = 0;

	void SetStats();

	void Display();
	void Reset();

	// Get methods
	int GetHealth() { return health; }
	void SetHealth(int _health) { health = _health; }
	int GetDamage() { return damage; }
	int GetRange() { return range; }
	int GetCritChance() { return crit_chance; }
	virtual sf::RectangleShape* GetShape() = 0;

	bool can_attack = true;
	bool can_move = true;

protected:
	int health = 1;
	int damage = 3;
	int range = 1; // Each tile is 64 by 64px
	int crit_chance = 50;
	int max_health = 20;
	sf::Vector2f start_pos = {100.f, 100.f};
	EntityTypes entity_type;
};

