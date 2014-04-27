#include "stdafx.h"
#include "FireAnt.h"
#include "Mouse.h"
#include "ProjectileAntBrain.h"

FireAnt::FireAnt(sf::Vector2f position, float speed, int round)
: BaseAnt(AntType::Ranged, position, speed)
{
	float modifier = (float)round / 5.0f;

	p_health = 30 + (30 * modifier);
	p_armour = 20 + (20 * modifier);
	p_proteinReward = 200 + (200 * modifier);
	p_meleeDamage = 0 + (0 * modifier);
	p_rangedDamage = 25 + (25 * modifier);
	p_speed = p_speed + (p_speed * modifier);

	p_render.setColor(sf::Color(171, 0, 11, 255));

	p_attackCooldown = sf::seconds(4);

	p_brain = new ProjectileAntBrain(this);
}

void FireAnt::update(sf::Time elapsed)
{
	BaseAnt::update(elapsed);

	p_brain->think();

	sf::Vector2f travelDistance = (p_velocity * p_speed) * elapsed.asSeconds();
	float travelLength = sqrt(pow(travelDistance.x, 2) + pow(travelDistance.y, 2));

	if (distanceToTarget() >= 200)
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

void FireAnt::handleEvent(sf::Event& e)
{

}

void FireAnt::handleCollision(std::list<sf::IntRect> walls)
{

}