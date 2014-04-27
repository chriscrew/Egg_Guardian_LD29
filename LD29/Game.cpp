#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "MeleeAnt.h"
#include "Mouse.h"
#include "BaseAttack.h"
#include "AntSpawner.h"
#include "SpawnManager.h"

#include <iomanip>
#include <sstream>

Game* Game::p_game = NULL;
Mouse* Mouse::p_mouse;

float getFPS(const sf::Time& time) {
	return (1000000.0f / time.asMicroseconds());
}

Game::Game()
: p_entityList()
, p_collisions()
, p_attacks()
, p_map()
, p_camera(sf::FloatRect(0, 0, 768, 576))
{
	p_map.resize(tileX * tileY);

	int temp[] =
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

	std::copy(std::begin(temp), std::end(temp), std::begin(p_map));

	p_game = this;

	p_isRunning = true;
	p_renderWindow = new sf::RenderWindow(sf::VideoMode(800, 600), "LD29 - Beneath the Surface");
	//p_renderWindow->setVerticalSyncEnabled(true);
	p_renderWindow->setKeyRepeatEnabled(false);

	p_camera.setCenter(384, 288);

	Mouse::p_mouse = new Mouse();

	p_roundTime = sf::seconds(10);
	p_currentRoundTime = sf::Time::Zero;
	p_currentRound = 1;

	p_restTime = sf::seconds(5);
	p_restTimeCounter = p_restTime;

	p_player = new Player(sf::Vector2f(400, 300), 80.0f);
	p_entityList.push_back(p_player);

	p_spawnManager = new SpawnManager();
	p_spawnManager->addSpawner(new AntSpawner(sf::Vector2f(96, 96), BaseAnt::AntType::Melee, sf::seconds(5)));
	p_spawnManager->start();

	p_dirttex.loadFromFile("dirtTiles.png");
	p_guiFont.loadFromFile("guiFont.ttf");

	generateMap();
}

Game::~Game()
{
	delete p_spawnManager;
	p_spawnManager = NULL;

	delete p_player;
	p_player = NULL;

	delete Mouse::p_mouse;
	Mouse::p_mouse = NULL;

	if (p_renderWindow)
	{
		p_renderWindow->close();
		delete p_renderWindow;
		p_renderWindow = NULL;
	}
}

void Game::start()
{
	sf::Clock clock;

	while (p_isRunning)
	{
		sf::Time elapsed = clock.restart();

		printf("FPS: %f\n", getFPS(elapsed));

		sf::Event e;

		while (p_renderWindow->pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
			{
				stop();
			}

			handleEvents(e);
		}

		Mouse::p_mouse->setMousePosition(*p_renderWindow);

		update(elapsed);
		render();
	}
}

void Game::stop()
{
	p_isRunning = false;
}

void Game::update(sf::Time elapsed)
{
	if (p_player->isDead())
		return;

	if (p_currentRound > MAX_ROUNDS)
		return;

	AttackList::iterator it = p_attacks.begin();

	while (it != p_attacks.end())
	{
		AttackList::iterator temp = it;
		++it;

		if (!(*temp)->hasTimedOut())
		{
			(*temp)->update(elapsed);
			(*temp)->handleCollision(p_collisions);
		}
		else
		{
			p_attacks.erase(temp);
		}
	}

	EntityList::iterator eit = p_entityList.begin();

	while (eit != p_entityList.end())
	{
		EntityList::iterator temp = eit;
		++eit;

		if (!(*temp)->isDead())
		{
			(*temp)->update(elapsed);
			(*temp)->handleCollision(p_collisions);

			for (BaseAttack* attack : p_attacks)
			{
				if (attack->getBounds().intersects((*temp)->getBounds()))
				{
					if (attack->canDamage((*temp)->getAntType()))
					{
						(*temp)->damage(attack->getDamage());
						attack->kill();
					}
				}
			}
		}
		else
		{
			if ((*temp)->getAntType() != BaseAnt::AntType::Player)
				p_player->increaseProtein((*temp)->getProteinReward());

			p_entityList.erase(temp);
		}
	}

	p_currentRoundTime += elapsed;

	if (p_currentRoundTime < p_roundTime)
	{
		p_spawnManager->update(elapsed);
	}
	else
	{
		p_spawnManager->stop();
		p_currentRoundTime = p_roundTime;

		p_restTimeCounter -= elapsed;

		if (p_restTimeCounter <= sf::seconds(0))
		{
			p_restTimeCounter = p_restTime;

			++p_currentRound;

			if (p_currentRound == 2)
			{
				p_map[22] = 3;
				generateMap();
				p_spawnManager->addSpawner(new AntSpawner(sf::Vector2f(672, 96), BaseAnt::AntType::Fast, sf::seconds(5)));
			}

			if (p_currentRound == 3)
			{
				p_map[85] = 3;
				generateMap();
				p_spawnManager->addSpawner(new AntSpawner(sf::Vector2f(96, 480), BaseAnt::AntType::Ranged, sf::seconds(5)));
			}

			if (p_currentRound == 4)
			{
				p_map[94] = 3;
				generateMap();
				p_spawnManager->addSpawner(new AntSpawner(sf::Vector2f(672, 480), BaseAnt::AntType::Melee, sf::seconds(5)));
			}

			p_currentRoundTime = sf::Time::Zero;
			p_spawnManager->start();
		}
	}
}

void Game::render()
{
	p_renderWindow->clear(sf::Color(63, 37, 24, 255));
	p_renderWindow->setView(p_camera);

	sf::RenderStates state;
	state.texture = &p_dirttex;

	p_renderWindow->draw(p_vertexMap, state);

	for (Entity* entity : p_entityList)
	{
		entity->draw(*p_renderWindow);
	}

	for (BaseAttack* projectile : p_attacks)
	{
		projectile->draw(*p_renderWindow);
	}

	p_renderWindow->setView(p_renderWindow->getDefaultView());

	sf::Text healthText;
	healthText.setFont(p_guiFont);
	healthText.setString("Health: " + std::to_string(p_player->getHealth()));
	healthText.setCharacterSize(15);
	healthText.setPosition(5, 5);

	sf::Text armourText;
	armourText.setFont(p_guiFont);
	armourText.setString("Armour: " + std::to_string(p_player->getArmour()));
	armourText.setCharacterSize(15);
	armourText.setPosition(5, 25);

	sf::Text proteinText;
	proteinText.setFont(p_guiFont);
	proteinText.setString("Protein: " + std::to_string(p_player->getProtein()));
	proteinText.setCharacterSize(15);
	proteinText.setPosition(5, 45);

	sf::Text roundTimeText;
	roundTimeText.setFont(p_guiFont);



	p_renderWindow->draw(healthText);
	p_renderWindow->draw(armourText);
	p_renderWindow->draw(proteinText);
	

	if (p_currentRound <= MAX_ROUNDS)
	{
		sf::Time roundLeft = p_roundTime - p_currentRoundTime;

		std::ostringstream out;
		out << std::setprecision(0) << round(roundLeft.asSeconds());

		roundTimeText.setString("Time Left: " + out.str() + " (Round " + std::to_string(p_currentRound) + ")");
		roundTimeText.setCharacterSize(15);
		roundTimeText.setPosition(300, 5);
		p_renderWindow->draw(roundTimeText);
	}

	if (p_currentRoundTime == p_roundTime && p_currentRound < MAX_ROUNDS)
	{
		sf::Text roundWaitText;
		roundWaitText.setFont(p_guiFont);

		std::ostringstream next;
		next << std::setprecision(0) << round(p_restTimeCounter.asSeconds());

		roundWaitText.setString("Next Round in: " + next.str());
		roundWaitText.setCharacterSize(50);
		roundWaitText.setPosition(96, 80);
		p_renderWindow->draw(roundWaitText);
	}

	if (p_player->isDead())
	{
		sf::Text gameOverText;
		gameOverText.setFont(p_guiFont);
		gameOverText.setString("Game Over");
		gameOverText.setCharacterSize(50);
		gameOverText.setPosition(96, 80);
		p_renderWindow->draw(gameOverText);
	}

	if (p_currentRound > MAX_ROUNDS)
	{
		sf::Text wonText;
		wonText.setFont(p_guiFont);
		wonText.setString("You Won");
		wonText.setCharacterSize(50);
		wonText.setPosition(96, 80);
		p_renderWindow->draw(wonText);
	}

	p_renderWindow->display();
}

void Game::handleEvents(sf::Event& e)
{
	for (Entity* entity : p_entityList)
	{
		entity->handleEvent(e);
	}
}

void Game::generateMap()
{
	p_collisions.clear();
	p_vertexMap = sf::VertexArray();

	p_vertexMap.setPrimitiveType(sf::Quads);
	p_vertexMap.resize(tileX * tileY * 4);

	for (unsigned int x = 0; x < tileX; ++x)
	{
		for (unsigned int y = 0; y < tileY; ++y)
		{
			int tileNumber = p_map[x + y * tileX];

			int tu = tileNumber % (256 / 64);
			int tv = tileNumber / (256 / 64);

			sf::Vertex* quad = &p_vertexMap[(x + y * tileX) * 4];

			quad[0].position = sf::Vector2f(x * 64, y * 64);
			quad[1].position = sf::Vector2f((x + 1) * 64, y * 64);
			quad[2].position = sf::Vector2f((x + 1) * 64, (y + 1) * 64);
			quad[3].position = sf::Vector2f(x * 64, (y + 1) * 64);

			quad[0].texCoords = sf::Vector2f(tu * 64, tv * 64);
			quad[1].texCoords = sf::Vector2f((tu + 1) * 64, tv * 64);
			quad[2].texCoords = sf::Vector2f((tu + 1) * 64, (tv + 1) * 64);
			quad[3].texCoords = sf::Vector2f(tu * 64, (tv + 1) * 64);

			if (tileNumber == 1)
			{
				sf::IntRect col = sf::IntRect((x * 64) + 1, (y * 64) + 1, 62, 62);
				p_collisions.push_back(col);
			}
		}
	}
}

void Game::addAttack(BaseAttack* attack)
{
	p_attacks.push_back(attack);
}

void Game::addAnt(BaseAnt* ant)
{
	p_entityList.push_back(ant);
}

sf::Vector2f Game::getPlayerPosition()
{
	return p_player->getPosition();
}