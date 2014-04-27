#pragma once

#include "BaseAnt.h"

class AntSpawner
{
public:
	AntSpawner(sf::Vector2f position, BaseAnt::AntType type, sf::Time rate);

	void update(sf::Time elapsed);
	void start();
	void stop();
	void reset();

private:
	sf::Vector2f p_position;
	sf::Time p_rate;
	sf::Time p_counter;
	BaseAnt::AntType p_spawnType;

	bool p_running;
};