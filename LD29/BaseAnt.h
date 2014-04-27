#pragma once

#include "Entity.h"
#include "Animation.h"

const float PI = 3.14159265;

class BaseAnt : public Entity
{
public:
	enum AntType
	{
		Player,
		Melee,
		Ranged,
		Fast
	};

public:

	BaseAnt(AntType type, sf::Vector2f position, float speed)
	{
		p_type = type;
		p_position = position;
		p_speed = speed;
		p_moveTarget = p_position;

		p_rotation = 0.0f;
		p_moving = false;
		p_velocity = sf::Vector2f(0.0f, 0.0f);

		p_walking = new Animation(0, 6, 7, 53, 50, sf::Time(sf::milliseconds(50)), 0);

		p_texture.loadFromFile("media/ant.png");

		p_render.setTexture(p_texture);
		p_render.setTextureRect(p_walking->getFrame());
		p_render.setPosition(p_position);
		p_render.setRotation(p_rotation);
		p_render.setOrigin(25, 26);

		p_attackCooldownCounter = sf::Time::Zero;
	}

	void update(sf::Time elapsed)
	{
		if (!canAttack())
			p_attackCooldownCounter -= elapsed;
	}

	virtual void draw(sf::RenderWindow& renderer)
	{
		p_render.setTextureRect(p_walking->getFrame());
		renderer.draw(p_render);
	}

	virtual void handleCollision(std::list<sf::IntRect> walls)
	{
		for (sf::IntRect wall : walls)
		{
			if (getBounds().intersects(wall))
				stop();
		}
	}

	void moveToTarget(sf::Vector2f target)
	{
		if (p_moveTarget != target)
		{
			p_moveTarget = target;
			sf::Vector2f distance = p_moveTarget - p_position;
			float length = sqrt(pow(distance.x, 2) + pow(distance.y, 2));
			p_velocity.x = distance.x / length;
			p_velocity.y = distance.y / length;

			rotateToTarget();

			p_moving = true;
			p_walking->start();
		}
	}

	void rotateToTarget()
	{
		float dx = p_moveTarget.x - p_position.x;
		float dy = p_moveTarget.y - p_position.y;

		float rotation = (atan2(dy, dx)) * 180 / PI;

		p_render.setRotation(rotation + 180);
	}

	float distanceToTarget()
	{
		sf::Vector2f distance = p_moveTarget - p_position;
		return sqrt(pow(distance.x, 2) + pow(distance.y, 2));
	}

	void stop()
	{
		{
			p_velocity = sf::Vector2f(0.0f, 0.0f);
			p_moving = false;
			p_walking->stop();
		}
	}

	sf::IntRect getBounds()
	{
		sf::IntRect bounds = sf::IntRect(p_render.getGlobalBounds());
		bounds.width -= 1;
		bounds.height -= 1;

		return bounds;
	}

	AntType getAntType() { return p_type; }

	sf::Vector2f getPosition() { return p_position; }

	void setSpeed(float speed) { p_speed = speed; }
	float getSpeed() { return p_speed; }

	void setHealth(int health) { p_health = health; }
	int getHealth() { return p_health; }

	void setMeleeDamage(int damage) { p_meleeDamage = damage; }
	int getMeleeDamage() { return p_meleeDamage; }

	void setRangedDamage(int damage) { p_rangedDamage = damage; }
	int getRangedDamage() { return p_rangedDamage; }

	int getProteinReward() { return p_proteinReward; }

	void damage(int amount)
	{
		if (p_armour > 0)
		{
			if (amount > p_armour)
			{
				amount -= p_armour;
				p_armour = 0;
			}
			else
			{
				p_armour -= amount;
				return;
			}
		}

		if (amount > 0)
		{
			p_health -= amount;
		}
	}

	bool isDead()
	{
		return p_health <= 0;
	}

	void setArmour(int armour) { p_armour = armour; }
	int getArmour() { return p_armour; }

	bool canAttack()
	{
		return p_attackCooldownCounter.asMilliseconds() <= 0;
	}

	void resetAttackCooldown()
	{
		p_attackCooldownCounter = p_attackCooldown;
	}

	sf::Vector2f getTarget()
	{
		return p_moveTarget;
	}

	void oppositeVelocity()
	{
		p_velocity = -p_velocity;
	}

protected:
	AntType p_type;

	sf::Vector2f p_position;
	float p_rotation;
	sf::Vector2f p_moveTarget;

	sf::Texture p_texture;
	sf::Sprite p_render;

	sf::Vector2f p_velocity;
	float p_speed;
	bool p_moving;
	Animation* p_walking;

	int p_health;
	int p_armour;

	sf::Time p_attackCooldown;
	sf::Time p_attackCooldownCounter;
	int p_meleeDamage;
	int p_rangedDamage;

	int p_proteinReward;
};