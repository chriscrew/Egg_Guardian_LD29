#pragma once

#include "BaseAttack.h"

#include <SFML\Graphics.hpp>

#include <string>

class Projectile : public BaseAttack
{
public:
	Projectile(BaseAnt* owner, OwnerGroup group, sf::Vector2f position, sf::Vector2f target, sf::Time timeout, std::string texture, int damage);
	~Projectile() {}

	virtual void update(sf::Time elapsed);

private:
	sf::Vector2f p_velocity;

};