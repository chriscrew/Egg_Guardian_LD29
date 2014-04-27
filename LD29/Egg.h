#pragma once

#include <SFML\Graphics.hpp>

#include "Entity.h"
#include "BaseAnt.h"

class Egg : Entity
{
public:
	Egg(sf::Vector2f position)
	{
		p_type = BaseAnt::AntType::Player;

		p_position = position;
		p_health = 1000;
		p_texture.loadFromFile("media/egg.png");

		p_render.setTexture(p_texture);
		p_render.setPosition(p_position);
		p_render.setOrigin(19, 19);
	}

	virtual void update(sf::Time elapsed) {};

	virtual void draw(sf::RenderWindow& renderer)
	{
		renderer.draw(p_render);
	}

	virtual void handleEvent(sf::Event& e) {}

	virtual void handleCollision(std::list<sf::IntRect> walls) {}

	int getHealth() { return p_health; }

	void damage(int amount)
	{
		p_health -= amount;
	}

	bool isDead()
	{
		return p_health <= 0;
	}

	sf::IntRect getBounds()
	{
		sf::IntRect bounds = sf::IntRect(p_render.getGlobalBounds());
		bounds.width -= 1;
		bounds.height -= 1;

		return bounds;
	}

	BaseAnt::AntType getAntType() { return p_type; }

	sf::Vector2f getPosition() { return p_position; }

public:
	BaseAnt::AntType p_type;

	sf::Vector2f p_position;
	int p_health;

	sf::Texture p_texture;
	sf::Sprite p_render;
};