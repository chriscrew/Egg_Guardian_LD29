#include "stdafx.h"
#include "MeleeAnt.h"
#include "Mouse.h"
#include "MeleeAntBrain.h"

MeleeAnt::MeleeAnt(sf::Vector2f position, float speed)
: BaseAnt(AntType::Melee, position, speed)
{
	p_health = 50;
	p_armour = 60;
	p_proteinReward = 100;

	p_meleeDamage = 15;
	p_rangedDamage = 0;

	p_render.setColor(sf::Color(93, 160, 238, 255));

	p_attackCooldown = sf::seconds(3);

	p_brain = new MeleeAntBrain(this);
}

void MeleeAnt::update(sf::Time elapsed)
{
	BaseAnt::update(elapsed);

	p_brain->think();

	sf::Vector2f travelDistance = (p_velocity * p_speed) * elapsed.asSeconds();
	float travelLength = sqrt(pow(travelDistance.x, 2) + pow(travelDistance.y, 2));

	if (distanceToTarget() > 35)
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

void MeleeAnt::handleEvent(sf::Event& e)
{

}

void MeleeAnt::handleCollision(std::list<sf::IntRect> walls)
{

}