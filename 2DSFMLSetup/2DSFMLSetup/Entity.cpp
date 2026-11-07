#include "Entity.h"

Entity::Entity() : entity_text(no_font, "NOT SET", 16)
{
}

Entity::~Entity()
{
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
		// "\nCan move: " + std::to_string(can_move)
	);
}
