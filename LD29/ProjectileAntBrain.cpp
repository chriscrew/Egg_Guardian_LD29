#include "stdafx.h"
#include "ProjectileAntBrain.h"
#include "Game.h"
#include "BaseAnt.h"
#include "Projectile.h"

ProjectileAntBrain::ProjectileAntBrain(BaseAnt* owner)
: BaseAntBrain(owner)
{
	think();
}

void ProjectileAntBrain::think()
{
	if (p_owner->distanceToTarget() < 200)
	{
		p_owner->oppositeVelocity();
	}
	{
		p_owner->moveToTarget(Game::p_game->getPlayerPosition());
	}

	if (p_owner->distanceToTarget() > 200 && p_owner->canAttack())
	{
		Game::p_game->addAttack(new Projectile(p_owner, BaseAttack::OwnerGroup::Computer, p_owner->getPosition(), Game::p_game->getPlayerPosition(), sf::seconds(3), "fire.png", p_owner->getRangedDamage()));
		p_owner->resetAttackCooldown();
	}
}