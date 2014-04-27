#include "stdafx.h"
#include "SpawnManager.h"
#include "AntSpawner.h"

SpawnManager::SpawnManager()
: p_spawners()
{

}

SpawnManager::~SpawnManager()
{
	for (AntSpawner* spawner : p_spawners)
	{
		delete spawner;
		spawner = NULL;
	}
}

void SpawnManager::update(sf::Time elapsed)
{
	for (AntSpawner* spawner : p_spawners)
	{
		spawner->update(elapsed);
	}
}

void SpawnManager::addSpawner(AntSpawner* spawner)
{
	p_spawners.push_back(spawner);
}

void SpawnManager::start(int round)
{
	for (AntSpawner* spawner : p_spawners)
	{
		spawner->start(round);
	}
}

void SpawnManager::stop()
{
	for (AntSpawner* spawner : p_spawners)
	{
		spawner->stop();
		spawner->reset();
	}
}

void SpawnManager::reset()
{
	for (AntSpawner* spawner : p_spawners)
	{
		spawner->reset();
	}
}

void SpawnManager::levelUp()
{
	for (AntSpawner* spawner : p_spawners)
	{

	}
}