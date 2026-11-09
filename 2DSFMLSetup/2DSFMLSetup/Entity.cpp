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
		range = 65.f;
		break;

	case ranged:
		damage = 5;
		range = 130.f;
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
		"\nRange: " + std::to_string((int)range / 64) + " tiles" +
		"\nCrit Chance: " + std::to_string(crit_chance) + "%"
	);
}

void Entity::Reset()
{
	GetShape().setPosition(start_pos);
	health = max_health;
	can_attack = true;
	can_move = true;
	UnsetTarget();
}
