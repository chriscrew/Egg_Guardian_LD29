#pragma once

#include "BaseAnt.h"

class ProjectileAntBrain;

class FireAnt : public BaseAnt
{
public:
	FireAnt(sf::Vector2f position, float speed, int round);
	~FireAnt();

	virtual void update(sf::Time elapsed);
	virtual void handleEvent(sf::Event& e);
	virtual void handleCollision(std::list<sf::IntRect> walls);

private:
	ProjectileAntBrain* p_brain;
};