#pragma once

#include "BaseAttack.h"

#include <string>

class MeleeAttack : public BaseAttack
{
public:
	MeleeAttack(OwnerGroup group, sf::Vector2f position, sf::Time timeout, std::string texture, int damage);
	~MeleeAttack() {}

	virtual void update(sf::Time elapsed);
};