#pragma once

class BaseAnt;
class BaseAttack;
class Player;
class SpawnManager;
class Egg;

#include <SFML\Graphics.hpp>

#include <list>
#include <vector>

const int tileX = 12;
const int tileY = 9;

const int MAX_ROUNDS = 5;

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

	sf::Vector2f getPlayerPosition();
	sf::Vector2f getEggPosition();

private:
	void update(sf::Time elapsed);
	void render();
	void handleEvents(sf::Event& e);

	void generateMap();

private:

	sf::View p_camera;

	bool p_isRunning;
	sf::RenderWindow* p_renderWindow;

	SpawnManager* p_spawnManager;

	EntityList p_entityList;
	Player* p_player;
	Egg* p_egg;

	AttackList p_attacks;

	sf::Texture p_dirttex;

	sf::VertexArray p_vertexMap;

	std::list<sf::IntRect> p_collisions;

	sf::Font p_guiFont;

	sf::Time p_roundTime;
	sf::Time p_currentRoundTime;
	sf::Time p_restTime;
	sf::Time p_restTimeCounter;

	int p_currentRound;

	std::vector<int> p_map;
};