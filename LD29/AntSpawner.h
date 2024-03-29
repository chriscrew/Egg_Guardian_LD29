#pragma once

#include "BaseAnt.h"

class AntSpawner
{
public:
	AntSpawner(sf::Vector2f position, sf::Time rate);

	void update(sf::Time elapsed);
	void start(int round);
	void stop();
	void reset();

private:
	sf::Vector2f p_position;
	sf::Time p_rate;
	sf::Time p_counter;

	bool p_running;
	int p_round;
};