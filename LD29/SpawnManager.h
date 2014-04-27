#pragma once

class AntSpawner;

#include <SFML\System\Time.hpp>

#include <list>

class SpawnManager
{
public:
	SpawnManager();
	~SpawnManager();

	void addSpawner(AntSpawner* spawner);
	void update(sf::Time elapsed);
	void start();
	void stop();
	void levelUp();

private:
	void reset();

private:
	std::list<AntSpawner*> p_spawners;
};