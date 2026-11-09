#include "Entity.h"

Entity::Entity(sf::Vector2f _start_pos) : entity_text(no_font, "NOT SET", 16)
{
	start_pos = _start_pos;
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
	);
}

void Entity::Reset()
{
	GetShape().setPosition(start_pos);
	// can_move = true;
	can_attack = true;
	health = max_health;
	UnsetTarget();
}
