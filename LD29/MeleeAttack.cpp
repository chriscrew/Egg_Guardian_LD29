#include "stdafx.h"
#include "MeleeAttack.h"
#include "Mouse.h"

MeleeAttack::MeleeAttack(OwnerGroup group, sf::Vector2f position, sf::Time timeout, std::string texture, int damage)
: BaseAttack(group, position, texture, damage)
{
	sf::Vector2f distance = sf::Vector2f(Mouse::p_mouse->getMousePosition()) - position;
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