#include "stdafx.h"
#include "MeleeAntBrain.h"
#include "Game.h"
#include "BaseAnt.h"
#include "MeleeAttack.h"

MeleeAntBrain::MeleeAntBrain(BaseAnt* owner)
: BaseAntBrain(owner)
{
	think();
}

void MeleeAntBrain::think()
{
	p_owner->moveToTarget(Game::p_game->getEggPosition());

	if (p_owner->distanceToTarget() <= 35 && p_owner->canAttack())
	{
		Game::p_game->addAttack(new MeleeAttack(p_owner, BaseAttack::OwnerGroup::Computer, p_owner->getPosition(), sf::milliseconds(500), "melee.png", p_owner->getMeleeDamage()));
		p_owner->resetAttackCooldown();
	}
}