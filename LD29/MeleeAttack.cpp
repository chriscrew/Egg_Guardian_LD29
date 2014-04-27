#include "stdafx.h"
#include "MeleeAttack.h"
#include "Mouse.h"

MeleeAttack::MeleeAttack(BaseAnt* owner, OwnerGroup group, sf::Vector2f position, sf::Time timeout, std::string texture, int damage)
: BaseAttack(owner, group, position, texture, damage)
{
	sf::Vector2f distance = owner->getTarget() - position;
	float length = sqrt(pow(distance.x, 2) + pow(distance.y, 2));

	sf::Vector2f direction;
	direction.x = (distance.x / length);
	direction.y = (distance.y / length);

	p_position += direction * 20.0f;

	p_timeLeft = timeout;
	p_render.setOrigin(5, 5);
	p_render.setPosition(p_position);
}

void MeleeAttack::update(sf::Time elapsed)
{
	BaseAttack::update(elapsed);
}