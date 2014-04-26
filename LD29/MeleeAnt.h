#pragma once

#include "BaseAnt.h"

class MeleeAnt : public BaseAnt
{
public:
	MeleeAnt(sf::Vector2f position, float speed);
	~MeleeAnt();

	virtual void update(sf::Time elapsed);
	virtual void handleEvent(sf::Event& e);
	virtual void handleCollision(std::list<sf::IntRect> walls);
};