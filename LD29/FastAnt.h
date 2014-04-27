#pragma once

#include "BaseAnt.h"

class FastAnt : public BaseAnt
{
public:
	FastAnt(sf::Vector2f position, float speed);
	~FastAnt();

	virtual void update(sf::Time elapsed);
	virtual void handleEvent(sf::Event& e);
	virtual void handleCollision(std::list<sf::IntRect> walls);
};