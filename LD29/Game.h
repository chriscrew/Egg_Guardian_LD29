#pragma once

class BaseAnt;
class Projectile;
class Player;

#include <SFML\Graphics.hpp>

#include <list>

const int tileX = 12;
const int tileY = 9;
const int map[] = 
{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

class Game
{
public:
	typedef std::list<BaseAnt*> EntityList;
	typedef std::list<Projectile*> ProjectileList;

public:
	static Game* p_game;

public:
	Game();
	~Game();

	void start();
	void stop();

	void addProjectile(Projectile* projectile);

private:
	void update(sf::Time elapsed);
	void render();
	void handleEvents(sf::Event& e);

	void generateMap();

private:
	bool p_isRunning;
	sf::RenderWindow* p_renderWindow;

	EntityList p_entityList;
	Player* p_player;

	ProjectileList p_projectiles;

	sf::Texture p_dirttex;

	sf::VertexArray p_vertexMap;

	std::list<sf::IntRect> p_collisions;

	sf::Font p_guiFont;
};