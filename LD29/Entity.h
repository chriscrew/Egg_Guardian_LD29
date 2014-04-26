#pragma once

#include <SFML\Graphics.hpp>

#include <list>

class Entity
{
public:
	virtual void update(sf::Time elapsed) = 0;
	virtual void draw(sf::RenderWindow& renderer) = 0;
	virtual void handleEvent(sf::Event& e) = 0;
	virtual void handleCollision(std::list<sf::IntRect> walls) = 0;
};