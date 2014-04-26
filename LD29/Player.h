#pragma once

#include "BaseAnt.h"

#include <list>

class Player : public BaseAnt
{

public:
	Player(sf::Vector2f position, float speed);
	~Player();

	virtual void update(sf::Time elapsed);
	virtual void handleEvent(sf::Event& e);

	void setProtein(int protein) { p_protein = protein; }
	void increaseProtein(int amount) { p_protein += amount; }
	void decreaseProtein(int amount) { p_protein -= amount; }
	int getProtein() { return p_protein; }

private:
	int p_protein;
};