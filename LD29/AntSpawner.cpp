#include "stdafx.h"
#include "AntSpawner.h"
#include "Game.h"
#include "MeleeAnt.h"
#include "FireAnt.h"
#include "FastAnt.h"

AntSpawner::AntSpawner(sf::Vector2f position, BaseAnt::AntType type, sf::Time rate)
{
	p_position = position;
	p_spawnType = type;
	p_rate = rate;
	p_running = false;
	reset();
}

void AntSpawner::update(sf::Time elapsed)
{
	if (p_running)
	{
		p_counter += elapsed;

		if (p_counter >= p_rate)
		{
			switch (p_spawnType)
			{
			case BaseAnt::Melee:
				Game::p_game->addAnt(new MeleeAnt(p_position, 40.0f));
				break;
			case BaseAnt::Ranged:
				Game::p_game->addAnt(new FireAnt(p_position, 30.0f));
				break;
			case BaseAnt::Fast:
				Game::p_game->addAnt(new FastAnt(p_position, 100.0f));
				break;
			default:
				break;
			}

			reset();
		}
	}
}

void AntSpawner::start()
{
	p_running = true;
}

void AntSpawner::stop()
{
	p_running = false;
}

void AntSpawner::reset()
{
	p_counter = sf::Time::Zero;
}