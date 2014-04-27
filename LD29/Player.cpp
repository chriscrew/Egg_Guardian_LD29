#include "stdafx.h"
#include "Player.h"
#include "Mouse.h"
#include "Game.h"
#include "Projectile.h"
#include "MeleeAttack.h"


Player::Player(sf::Vector2f position, float speed)
: BaseAnt(AntType::Player, position, speed)
{
	p_health = 100;
	p_armour = 100;
	p_protein = 0;

	p_meleeDamage = 10;
	p_rangedDamage = 30;

	p_render.setColor(sf::Color(236, 190, 19, 255));
}

Player::~Player()
{

}

void Player::update(sf::Time elapsed)
{
	rotateToTarget(sf::Vector2f(Mouse::p_mouse->getMousePosition()));

	sf::Vector2f travelDistance = (p_velocity * p_speed) * elapsed.asSeconds();
	float travelLength = sqrt(pow(travelDistance.x, 2) + pow(travelDistance.y, 2));

	if (travelLength < distanceToTarget())
	{
		p_position += travelDistance;
	}
	else
	{
		stop();
	}
	
	p_render.setPosition(p_position);

	if (p_moving)
	{
		p_walking->update(elapsed);
	}
}

void Player::handleEvent(sf::Event& e)
{
	if (e.type == sf::Event::MouseButtonPressed)
	{
		if (e.mouseButton.button == sf::Mouse::Left)
		{
			moveToTarget(sf::Vector2f(Mouse::p_mouse->getMousePosition()));
		}

		if (e.mouseButton.button == sf::Mouse::Right)
		{
			Game::p_game->addAttack(new MeleeAttack(BaseAttack::OwnerGroup::Player, p_position, sf::milliseconds(500), "melee.png", p_meleeDamage));
		}
	}

	if (e.type == sf::Event::KeyPressed)
	{
		if (e.key.code == sf::Keyboard::Space)
		{
			Game::p_game->addAttack(new Projectile(BaseAttack::OwnerGroup::Player, p_position, sf::Vector2f(Mouse::p_mouse->getMousePosition()), sf::seconds(3), "fire.png", p_rangedDamage));
		}
	}
}