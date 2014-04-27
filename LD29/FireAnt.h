#pragma once

#include "BaseAnt.h"

class FireAnt : public BaseAnt
{
public:
	FireAnt(sf::Vector2f position, float speed);
	~FireAnt();

	virtual void update(sf::Time elapsed);
	virtual void handleEvent(sf::Event& e);
	virtual void handleCollision(std::list<sf::IntRect> walls);
};