#pragma once

#include "BaseAnt.h"

#include <SFML\Graphics.hpp>

#include <string>

class Projectile
{
public:
	enum OwnerGroup
	{
		Player,
		Computer
	};

public:
	Projectile(OwnerGroup group, sf::Vector2f position, sf::Vector2f target, sf::Time timeout, std::string texture, int damage);

	void update(sf::Time elapsed);
	void draw(sf::RenderWindow& renderer);
	void handleCollision(std::list<sf::IntRect> walls);

	bool hasTimedOut()
	{
		return p_timeLeft.asMilliseconds() <= 0;
	}

	sf::IntRect getBounds()
	{
		sf::IntRect bounds = sf::IntRect(p_render.getGlobalBounds());

		return bounds;
	}

	void kill()
	{
		p_timeLeft = sf::milliseconds(0);
	}

	bool canDamage(BaseAnt::AntType antType)
	{
		if (p_ownerGroup == OwnerGroup::Player && !(antType == BaseAnt::AntType::Player))
			return true;

		return false;
	}

	int getDamage() { return p_damage; }

private:
	OwnerGroup p_ownerGroup;

	sf::Vector2f p_position;
	sf::Vector2f p_velocity;
	sf::Texture p_texture;
	sf::Sprite p_render;

	sf::Time p_timeLeft;

	int p_damage;
};