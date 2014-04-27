#include "stdafx.h"
#include "AntSpawner.h"
#include "Game.h"
#include "MeleeAnt.h"

AntSpawner::AntSpawner(sf::Vector2f position, BaseAnt::AntType type, sf::Time rate)
{
	p_position = position;
	p_spawnType = type;
	p_rate = rate;
	p_counter = sf::Time::Zero;
}

void AntSpawner::update(sf::Time elapsed)
{
	p_counter += elapsed;

	if (p_counter >= p_rate)
	{
		switch (p_spawnType)
		{
		case BaseAnt::Melee:
			Game::p_game->addAnt(new MeleeAnt(p_position, 80.0f));
			break;
		case BaseAnt::Ranged:
			break;
		default:
			break;
		}

		p_counter = sf::Time::Zero;
	}
}