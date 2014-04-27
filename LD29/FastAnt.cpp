#include "stdafx.h"
#include "FastAnt.h"
#include "Mouse.h"

FastAnt::FastAnt(sf::Vector2f position, float speed)
: BaseAnt(AntType::Ranged, position, speed)
{
	p_health = 15;
	p_armour = 10;
	p_proteinReward = 150;

	p_meleeDamage = 10;
	p_rangedDamage = 0;

	p_render.setColor(sf::Color(44, 41, 24, 255));
}

void FastAnt::update(sf::Time elapsed)
{
	rotateToTarget(p_moveTarget);

	sf::Vector2f travelDistance = (p_velocity * p_speed) * elapsed.asSeconds();
	float travelLength = sqrt(pow(travelDistance.x, 2) + pow(travelDistance.y, 2));

	if (travelLength < distanceToTarget())
	{
		p_position += travelDistance;
	}
	else
	{
		stop();
	}

	p_render.setPosition(p_position);

	if (p_moving)
	{
		p_walking->update(elapsed);
	}
}

void FastAnt::handleEvent(sf::Event& e)
{

}

void FastAnt::handleCollision(std::list<sf::IntRect> walls)
{

}