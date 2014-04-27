#include "stdafx.h"
#include "FastAnt.h"
#include "Mouse.h"

FastAnt::FastAnt(sf::Vector2f position, float speed, int round)
: MeleeAnt(position, speed, round)
{
	float modifier = (float)round / 5.0f;

	p_health = 15 + (15 * modifier);
	p_armour = 10 + (10 * modifier);
	p_proteinReward = 150 + (150 * modifier);
	p_meleeDamage = 2 + (2 * modifier);
	p_rangedDamage = 0 + (0 * modifier);

	p_render.setColor(sf::Color(44, 41, 24, 255));

	p_attackCooldown = sf::seconds(2);
}