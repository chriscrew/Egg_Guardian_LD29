#pragma once

class BaseAnt;
class BaseAttack;
class Player;
class SpawnManager;

#include <SFML\Graphics.hpp>

#include <list>

const int tileX = 12;
const int tileY = 9;
const int map[] = 
{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
1, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 1,
1, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 1,
1, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 1,
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

class Game
{
public:
	typedef std::list<BaseAnt*> EntityList;
	typedef std::list<BaseAttack*> AttackList;

public:
	static Game* p_game;

public:
	Game();
	~Game();

	void start();
	void stop();

	void addAttack(BaseAttack* attack);
	void addAnt(BaseAnt* ant);

private:
	void update(sf::Time elapsed);
	void render();
	void handleEvents(sf::Event& e);

	void generateMap();

private:
	bool p_isRunning;
	sf::RenderWindow* p_renderWindow;

	SpawnManager* p_spawnManager;

	EntityList p_entityList;
	Player* p_player;

	AttackList p_attacks;

	sf::Texture p_dirttex;

	sf::VertexArray p_vertexMap;

	std::list<sf::IntRect> p_collisions;

	sf::Font p_guiFont;

	sf::Time p_roundTime;
	sf::Time p_currentRoundTime;
};