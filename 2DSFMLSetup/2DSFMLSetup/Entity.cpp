#include "Entity.h"

Entity::Entity() : entity_text(no_font, "NOT SET", 16)
{
}

Entity::~Entity()
{
}

void Entity::Special()
{
	if (can_special)
	{
		switch (entity_type)
		{
		case melee:
			std::cout << "Damage increase";
			damage += 2;

			break;

		case ranged:
			std::cout << "Area attack";
			break;

		case tank:
			// healing
			if (health < max_health)
			{
				health += 5;
			}

			if (health > max_health)
			{
				health = max_health;
			}

			break;

		case super:
			//Enhance

			damage *= 2;
			range++;
		}
	}

	can_special = false;
}

// Combat roles here
void Entity::SetStats()
{
	switch (entity_type)
	{
	case melee:
		start_dmg = 10;
		range = 1;
		max_health = 150;
		crit_chance = 75;
		break;

	case ranged:
		start_dmg = 5;
		range = 2;
		max_health = 100;
		crit_chance = 50;
		break;

	case tank:
		start_dmg = 5;
		range = 1;
		max_health = 300;
		crit_chance = 25;
		break;

	case super:
		start_dmg = 2;
		range = 1;
		max_health = 1000;
		crit_chance = 100;
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
		"\nHealth: " + std::to_string(health) + "/" + std::to_string(max_health) +
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
	damage = start_dmg;

	can_attack = true;
	can_move = true;
	can_special = true;

	UnsetTarget();
}
