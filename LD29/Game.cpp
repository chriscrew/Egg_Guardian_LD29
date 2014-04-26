#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "MeleeAnt.h"
#include "Mouse.h"
#include "Projectile.h"

Game* Game::p_game = NULL;
Mouse* Mouse::p_mouse;

float getFPS(const sf::Time& time) {
	return (1000000.0f / time.asMicroseconds());
}

Game::Game()
: p_entityList()
, p_collisions()
, p_projectiles()
{
	p_game = this;

	p_isRunning = true;
	p_renderWindow = new sf::RenderWindow(sf::VideoMode(800, 600), "LD29 - Beneath the Surface");
	//p_renderWindow->setVerticalSyncEnabled(true);

	Mouse::p_mouse = new Mouse();

	p_player = new Player(sf::Vector2f(400, 300), 80.0f);
	p_entityList.push_back(p_player);
	p_entityList.push_back(new MeleeAnt(sf::Vector2f(200, 175), 50.0f));
	p_entityList.push_back(new MeleeAnt(sf::Vector2f(300, 175), 50.0f));
	p_entityList.push_back(new MeleeAnt(sf::Vector2f(400, 175), 50.0f));
	p_entityList.push_back(new MeleeAnt(sf::Vector2f(500, 175), 50.0f));
	p_entityList.push_back(new MeleeAnt(sf::Vector2f(600, 175), 50.0f));

	p_dirttex.loadFromFile("dirtTiles.png");
	p_guiFont.loadFromFile("guiFont.ttf");

	generateMap();
}

Game::~Game()
{
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
	ProjectileList::iterator it = p_projectiles.begin();

	while (it != p_projectiles.end())
	{
		ProjectileList::iterator temp = it;
		++it;

		if (!(*temp)->hasTimedOut())
		{
			(*temp)->update(elapsed);
			(*temp)->handleCollision(p_collisions);
		}
		else
		{
			p_projectiles.erase(temp);
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

			for (Projectile* projectile : p_projectiles)
			{
				if (projectile->getBounds().intersects((*temp)->getBounds()))
				{
					if (projectile->canDamage((*temp)->getAntType()))
					{
						(*temp)->damage(projectile->getDamage());
						projectile->kill();
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

	for (Projectile* projectile : p_projectiles)
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

	p_renderWindow->draw(healthText);
	p_renderWindow->draw(armourText);
	p_renderWindow->draw(proteinText);

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

			int tu = tileNumber % (128 / 64);
			int tv = tileNumber / (128 / 64);

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

void Game::addProjectile(Projectile* projectile)
{
	p_projectiles.push_back(projectile);
}