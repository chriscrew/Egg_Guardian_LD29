#pragma once

#include <SFML\Graphics.hpp>

class Mouse
{
public:
	static Mouse* p_mouse;

public:
	void setMousePosition(sf::RenderWindow& renderer)
	{
		p_position = sf::Mouse::getPosition(renderer);
	}

	sf::Vector2i getMousePosition() { return p_position;  }

private:
	sf::Vector2i p_position;

};