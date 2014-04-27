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
{
	p_game = this;

	p_isRunning = true;
	p_renderWindow = new sf::RenderWindow(sf::VideoMode(800, 600), "LD29 - Beneath the Surface");
	//p_renderWindow->setVerticalSyncEnabled(true);
	p_renderWindow->setKeyRepeatEnabled(false);

	Mouse::p_mouse = new Mouse();

	p_roundTime = sf::seconds(15);
	p_currentRoundTime = sf::Time::Zero;

	p_player = new Player(sf::Vector2f(400, 300), 80.0f);
	p_entityList.push_back(p_player);

	p_spawnManager = new SpawnManager();
	p_spawnManager->addSpawner(new AntSpawner(sf::Vector2f(200, 175), BaseAnt::AntType::Fast, sf::seconds(5)));
	p_spawnManager->addSpawner(new AntSpawner(sf::Vector2f(400, 175), BaseAnt::AntType::Melee, sf::seconds(5)));
	p_spawnManager->addSpawner(new AntSpawner(sf::Vector2f(600, 175), BaseAnt::AntType::Ranged, sf::seconds(5)));
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

	p_currentRoundTime += elapsed;

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

	if (p_currentRoundTime < p_roundTime)
	{
		p_spawnManager->update(elapsed);
	}
	else
	{
		p_spawnManager->stop();
		p_currentRoundTime = p_roundTime;
	}
	
}

void Game::render()
{
	p_renderWindow->clear(sf::Color(63, 37, 24, 255));
	p_renderWindow->setView(p_renderWindow->getDefaultView());

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
	proteinText.setFont(p_guiFont);

	sf::Time roundLeft = p_roundTime - p_currentRoundTime;

	std::ostringstream out;
	out << std::setprecision(0) << round(roundLeft.asSeconds());

	proteinText.setString("Time Left: " + out.str());
	proteinText.setCharacterSize(15);
	proteinText.setPosition(400, 5);

	p_renderWindow->draw(healthText);
	p_renderWindow->draw(armourText);
	p_renderWindow->draw(proteinText);

	if (p_player->isDead())
	{
		sf::Text gameOverText;
		gameOverText.setFont(p_guiFont);
		gameOverText.setString("Game Over");
		gameOverText.setCharacterSize(50);
		gameOverText.setPosition(400, 300);
		p_renderWindow->draw(gameOverText);
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
	p_vertexMap.setPrimitiveType(sf::Quads);
	p_vertexMap.resize(tileX * tileY * 4);

	for (unsigned int x = 0; x < tileX; ++x)
	{
		for (unsigned int y = 0; y < tileY; ++y)
		{
			int tileNumber = map[x + y * tileX];

			int tu = tileNumber % (192 / 64);
			int tv = tileNumber / (192 / 64);

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
	ant->moveToTarget(p_player->getPosition());
	p_entityList.push_back(ant);
}