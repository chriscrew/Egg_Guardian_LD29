#include "stdafx.h"
#include "AntSpawner.h"
#include "Game.h"
#include "MeleeAnt.h"
#include "FireAnt.h"
#include "FastAnt.h"

AntSpawner::AntSpawner(sf::Vector2f position, sf::Time rate)
{
	p_position = position;
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
			int spawnType = rand() % (4 - 1) + 1;

			switch (spawnType)
			{
			case BaseAnt::Melee:
				Game::p_game->addAnt(new MeleeAnt(p_position, 40.0f, p_round));
				break;
			case BaseAnt::Ranged:
				Game::p_game->addAnt(new FireAnt(p_position, 30.0f, p_round));
				break;
			case BaseAnt::Fast:
				Game::p_game->addAnt(new FastAnt(p_position, 100.0f, p_round));
				break;
			default:
				break;
			}

			reset();
		}
	}
}

void AntSpawner::start(int round)
{
	p_running = true;
	p_round = round;
	p_counter = p_rate;
}

void AntSpawner::stop()
{
	p_running = false;
}

void AntSpawner::reset()
{
	p_counter = sf::Time::Zero;
}