#include "stdafx.h"
#include "Projectile.h"

Projectile::Projectile(OwnerGroup group, sf::Vector2f position, sf::Vector2f target, sf::Time timeout, std::string texture, int damage)
{
	p_ownerGroup = group;

	sf::Vector2f distance = target - position;
	float length = sqrt(pow(distance.x, 2) + pow(distance.y, 2));
	p_velocity.x = (distance.x / length);
	p_velocity.y = (distance.y / length);

	p_position = position;
	p_position += p_velocity * 30.0f;

	p_texture.loadFromFile(texture);

	p_render.setTexture(p_texture);
	p_render.setPosition(p_position);
	p_render.setOrigin(10, 10);

	float dx = target.x - p_position.x;
	float dy = target.y - p_position.y;

	float rotation = (atan2(dy, dx)) * 180 / 3.14159265;

	p_render.setRotation(rotation + 180);

	p_timeLeft = timeout;
	p_damage = damage;
}

void Projectile::update(sf::Time elapsed)
{
	p_timeLeft -= elapsed;
	p_position += (p_velocity  * 140.0f) * elapsed.asSeconds();
	p_render.setPosition(p_position);

}

void Projectile::draw(sf::RenderWindow& renderer)
{
	renderer.draw(p_render);
}

void Projectile::handleCollision(std::list<sf::IntRect> walls)
{
	for (sf::IntRect wall : walls)
	{
		if (getBounds().intersects(wall))
			kill();
	}
}