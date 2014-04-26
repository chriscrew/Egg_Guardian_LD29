#include "stdafx.h"
#include "MeleeAnt.h"
#include "Mouse.h"

MeleeAnt::MeleeAnt(sf::Vector2f position, float speed)
: BaseAnt(AntType::Melee, position, speed)
{
	p_health = 15;
	p_armour = 10;
	p_proteinReward = 100;

	p_render.setColor(sf::Color(93, 160, 238, 255));
}

void MeleeAnt::update(sf::Time elapsed)
{

}

void MeleeAnt::handleEvent(sf::Event& e)
{

}

void MeleeAnt::handleCollision(std::list<sf::IntRect> walls)
{

}