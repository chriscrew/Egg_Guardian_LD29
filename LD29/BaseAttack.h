#pragma once

#include "BaseAnt.h"

#include <SFML\Graphics.hpp>

#include <string>

class BaseAttack
{
public:
	enum OwnerGroup
	{
		Player,
		Computer
	};

public:
	BaseAttack(BaseAnt* owner, OwnerGroup group, sf::Vector2f position, std::string texture, int damage)
	{
		p_owner = owner;
		p_ownerGroup = group;
		p_position = position;
		p_damage = damage;

		p_texture.loadFromFile(texture);

		p_render.setTexture(p_texture);
		p_render.setPosition(p_position);
	}

	virtual void update(sf::Time elapsed)
	{
		p_timeLeft -= elapsed;
	}

	virtual void draw(sf::RenderWindow& renderer)
	{
		renderer.draw(p_render);
	}

	virtual void handleCollision(std::list<sf::IntRect> walls)
	{
		for (sf::IntRect wall : walls)
		{
			if (getBounds().intersects(wall))
				kill();
		}
	}

	bool hasTimedOut()
	{
		return p_timeLeft.asMilliseconds() <= 0;
	}

	sf::IntRect getBounds()
	{
		sf::IntRect bounds = sf::IntRect(p_render.getGlobalBounds());

		return bounds;
	}

	int getDamage() { return p_damage; }

	bool canDamage(BaseAnt::AntType antType)
	{
		if (p_ownerGroup == OwnerGroup::Player && !(antType == BaseAnt::AntType::Player))
			return true;

		if (p_ownerGroup == OwnerGroup::Computer && antType == BaseAnt::AntType::Player)
			return true;

		return false;
	}

	virtual void kill()
	{
		p_timeLeft = sf::milliseconds(0);
		p_damage = 0;
	}


protected:
	BaseAnt* p_owner;
	OwnerGroup p_ownerGroup;

	sf::Vector2f p_position;
	sf::Texture p_texture;
	sf::Sprite p_render;

	sf::Time p_timeLeft;

	int p_damage;
};