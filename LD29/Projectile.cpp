#include "stdafx.h"
#include "Projectile.h"

Projectile::Projectile(BaseAnt* owner, OwnerGroup group, sf::Vector2f position, sf::Vector2f target, sf::Time timeout, std::string texture, int damage)
: BaseAttack(owner,group, position, texture, damage)
{

	p_render.setOrigin(10, 10);

	sf::Vector2f distance = target - position;
	float length = sqrt(pow(distance.x, 2) + pow(distance.y, 2));
	p_velocity.x = (distance.x / length);
	p_velocity.y = (distance.y / length);

	p_position += p_velocity * 30.0f;

	float dx = target.x - p_position.x;
	float dy = target.y - p_position.y;

	float rotation = (atan2(dy, dx)) * 180 / 3.14159265;

	p_render.setRotation(rotation + 180);

	p_timeLeft = timeout;
}

void Projectile::update(sf::Time elapsed)
{
	BaseAttack::update(elapsed);

	p_position += (p_velocity  * 140.0f) * elapsed.asSeconds();
	p_render.setPosition(p_position);

}