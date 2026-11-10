#include "Entity.h"

Entity::Entity() : entity_text(no_font, "NOT SET", 16)
{
}

Entity::~Entity()
{
}

// Combat roles here
void Entity::SetStats()
{
	switch (entity_type)
	{
	case melee:
		damage = 10;
		range = 1;
		max_health = 150;
		crit_chance = 75;
		break;

	case ranged:
		damage = 5;
		range = 2;
		max_health = 100;
		crit_chance = 50;
		break;

	case tank:
		damage = 5;
		range = 1;
		max_health = 300;
		crit_chance = 25;
		break;

	default:
		break;
	}
}

// Communicate the stats to the user
void Entity::Display()
{
	entity_text.setString(
		"Stats: "
		"\nHealth: " + std::to_string(health) +
		"\nDamage: " + std::to_string(damage) +
		"\nRange: " + std::to_string(range) + " tiles" +
		"\nCrit Chance: " + std::to_string(crit_chance) + "%"
	);
}

// Reset all enititys back to their default state and position
void Entity::Reset()
{
	GetShape()->setPosition(start_pos);
	health = max_health;
	can_attack = true;
	can_move = true;
	UnsetTarget();
}
