#include "stdafx.h"
#include "FastAnt.h"
#include "Mouse.h"

FastAnt::FastAnt(sf::Vector2f position, float speed)
: MeleeAnt(position, speed)
{
	p_health = 15;
	p_armour = 10;
	p_proteinReward = 150;

	p_meleeDamage = 1;
	p_rangedDamage = 0;

	p_render.setColor(sf::Color(44, 41, 24, 255));

	p_attackCooldown = sf::seconds(2);
}