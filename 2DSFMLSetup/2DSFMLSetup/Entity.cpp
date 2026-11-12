#include "Entity.h"

Entity::Entity() : entity_text(no_font, "NOT SET", 16)
{
}

Entity::~Entity()
{
}

// Activate special ability
void Entity::Special()
{
	if (can_special && health > 0)
	{
		switch (entity_type)
		{
		case melee:
			if (damage > 5)
			{
				damage--;
			}

			if (crit_chance < 100)
			{
				crit_chance += 5;
			}

			break;

		case ranged:
			if (damage < 20)
			{
				damage += 2;
			}
			
			if (crit_chance > 0)
			{
				crit_chance -= 5;
			}

			break;

		case tank:
			if (damage > 2)
			{
				damage -= 2;
			}

			if (health < max_health)
			{
				health += 10;
			}

			if (health > max_health)
			{
				health = max_health;
			}

			break;

		case super:
			damage *= 2;
			if (crit_chance < 100)
			{
				crit_chance += 5;
			}
			range++;

			break;

		default:
			break;
		}
	}

	can_special = false;
}

// Adjust stats here
void Entity::SetStats()
{
	switch (entity_type)
	{
	case melee:
		start_dmg = 10;
		range = 1;
		max_health = 75;
		crit_chance = 75;
		break;

	case ranged:
		start_dmg = 6;
		range = 2;
		max_health = 50;
		crit_chance = 50;
		break;

	case tank:
		start_dmg = 6;
		range = 1;
		max_health = 100;
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
	if (health < 0) { health = 0; }

	std::string entity_string = 
	"Health: " + std::to_string(health) + "/" + std::to_string(max_health) +
	"\nDamage: " + std::to_string(damage) +
	"\nRange: " + std::to_string(range) + " tiles" +
	"\nCrit Chance: " + std::to_string(crit_chance) + "%" +
	"\nAbility: ";

	switch (entity_type)
	{
	case melee:
		entity_string += "Damage down, Crit chance up";
		break;

	case ranged:
		entity_string += "Damage up, Crit chance down";
		break;

	case tank:
		entity_string += "Damage down, Heal (Not over max)";
		break;

	case super:
		entity_string += "Enhance all stats";
	}

	entity_text.setString(entity_string);
}

// Reset all enititys back to their default state and position
void Entity::Reset()
{
	GetShape()->setPosition(start_pos);
	
	health = max_health;
	damage = start_dmg;

	CanAction();

	UnsetTarget();
}

// Gets all their moves back
void Entity::CanAction()
{
	can_attack = true;
	can_move = true;
	can_special = true;
}
