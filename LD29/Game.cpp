#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "MeleeAnt.h"
#include "Mouse.h"
#include "BaseAttack.h"
#include "AntSpawner.h"
#include "SpawnManager.h"
#include "Egg.h"

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
	p_showHelpMenu = true;
	p_roundWaiting = false;

	p_map.resize(tileX * tileY);

	int temp[] =
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
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

	p_egg = new Egg(sf::Vector2f(416, 288));

	p_player = new Player(sf::Vector2f(352, 288), 80.0f);
	p_entityList.push_back(p_player);

	p_spawnManager = new SpawnManager();
	p_spawnManager->addSpawner(new AntSpawner(sf::Vector2f(96, 96), sf::seconds(5)));
	p_spawnManager->start(p_currentRound);

	p_dirttex.loadFromFile("media/dirtTiles.png");
	p_guiFont.loadFromFile("media/guiFont.ttf");

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

			if (e.type == sf::Event::KeyPressed)
			{
				if (e.key.code == sf::Keyboard::Escape)
				{
					stop();
				}

				if (p_showHelpMenu)
				{
					if (e.key.code == sf::Keyboard::Return)
					{
						p_showHelpMenu = false;
					}
				}
			}

			if (!p_showHelpMenu)
			{
				handleEvents(e);
			}
		}

		if (p_showHelpMenu)
		{
			renderHelpMenu();
		}

		Mouse::p_mouse->setMousePosition(*p_renderWindow);

		if (!p_showHelpMenu)
		{
			update(elapsed);
			render();
		}
	}
}

void Game::stop()
{
	p_isRunning = false;
}

void Game::update(sf::Time elapsed)
{
	if (p_egg->isDead())
		return;

	if (p_player->isDead())
		return;

	if (p_currentRound > MAX_ROUNDS && p_entityList.size() == 1)
		return;

	if (p_roundWaiting)
		p_restTimeCounter -= elapsed;

	AttackList::iterator it = p_attacks.begin();

	while (it != p_attacks.end())
	{
		AttackList::iterator temp = it;
		++it;

		if (!(*temp)->hasTimedOut())
		{
			(*temp)->update(elapsed);
			(*temp)->handleCollision(p_collisions);

			if (!p_egg->isDead())
			{
				if ((*temp)->getBounds().intersects(p_egg->getBounds()))
				{
					if ((*temp)->canDamage(p_egg->getAntType()))
					{
						p_egg->damage((*temp)->getDamage());
						(*temp)->kill();
					}
				}
			}

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
		p_roundWaiting = true;
	}

	if (p_roundWaiting)
		nextRound();
}

void Game::nextRound()
{
	p_spawnManager->stop();
	p_currentRoundTime = p_roundTime;

	if (p_restTimeCounter <= sf::seconds(0))
	{
		p_restTimeCounter = p_restTime;

		++p_currentRound;

		p_player->levelUp(p_currentRound);

		if (p_currentRound == 2)
		{
			p_map[22] = 3;
			generateMap();
			p_spawnManager->addSpawner(new AntSpawner(sf::Vector2f(672, 96), sf::seconds(4)));
		}

		if (p_currentRound == 3)
		{
			p_map[85] = 3;
			generateMap();
			p_spawnManager->addSpawner(new AntSpawner(sf::Vector2f(96, 480), sf::seconds(4)));
		}

		if (p_currentRound == 4)
		{
			p_map[94] = 3;
			generateMap();
			p_spawnManager->addSpawner(new AntSpawner(sf::Vector2f(672, 480), sf::seconds(3)));
		}

		p_currentRoundTime = sf::Time::Zero;

		if (p_currentRound <= MAX_ROUNDS)
			p_spawnManager->start(p_currentRound);

		p_roundWaiting = false;
	}
}

void Game::render()
{
	p_renderWindow->clear(sf::Color(63, 37, 24, 255));
	p_renderWindow->setView(p_camera);

	sf::RenderStates state;
	state.texture = &p_dirttex;

	p_renderWindow->draw(p_vertexMap, state);

	p_egg->draw(*p_renderWindow);

	for (Entity* entity : p_entityList)
	{
		entity->draw(*p_renderWindow);
	}

	for (BaseAttack* projectile : p_attacks)
	{
		projectile->draw(*p_renderWindow);
	}

	p_renderWindow->setView(p_renderWindow->getDefaultView());

	sf::Text eggHealthText;
	eggHealthText.setFont(p_guiFont);
	eggHealthText.setString("Egg Health: " + std::to_string(p_egg->getHealth()));
	eggHealthText.setCharacterSize(12);
	eggHealthText.setPosition(5, 5);

	sf::Text healthText;
	healthText.setFont(p_guiFont);
	healthText.setString("Health: " + std::to_string(p_player->getHealth()));
	healthText.setCharacterSize(12);
	healthText.setPosition(5, 20);

	sf::Text armourText;
	armourText.setFont(p_guiFont);
	armourText.setString("Armour: " + std::to_string(p_player->getArmour()));
	armourText.setCharacterSize(12);
	armourText.setPosition(5, 35);

	sf::Text roundTimeText;
	roundTimeText.setFont(p_guiFont);

	p_renderWindow->draw(eggHealthText);
	p_renderWindow->draw(healthText);
	p_renderWindow->draw(armourText);


	if (p_player->isDead() || p_egg->isDead())
	{
		sf::Text gameOverText;
		gameOverText.setFont(p_guiFont);
		gameOverText.setString("Game Over");
		gameOverText.setCharacterSize(50);
		gameOverText.setPosition(230, 80);
		p_renderWindow->draw(gameOverText);
	}
	else if (p_currentRound > MAX_ROUNDS && p_entityList.size() == 1)
	{

		sf::Text wonText;
		wonText.setFont(p_guiFont);
		wonText.setString("You Won");
		wonText.setCharacterSize(50);
		wonText.setPosition(230, 80);
		p_renderWindow->draw(wonText);
	}
	else
	{

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
			roundWaitText.setPosition(150, 80);
			p_renderWindow->draw(roundWaitText);
		}
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

			int tu = tileNumber % (320 / 64);
			int tv = tileNumber / (320 / 64);

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

sf::Vector2f Game::getEggPosition()
{
	return p_egg->getPosition();
}

void Game::renderHelpMenu()
{
	p_renderWindow->clear(sf::Color(63, 37, 24, 255));
	p_renderWindow->setView(p_camera);

	sf::Text titleText;
	titleText.setFont(p_guiFont);
	titleText.setString("Egg Guardian");
	titleText.setCharacterSize(50);
	titleText.setPosition(200, 100);
	p_renderWindow->draw(titleText);

	sf::Text infoText;
	infoText.setFont(p_guiFont);
	infoText.setString("Protect the last of your ants colonies from invaders \nLeft click to move \nRight click for melee \nSpace to shoot");
	infoText.setCharacterSize(15);
	infoText.setPosition(150, 200);
	p_renderWindow->draw(infoText);

	sf::Texture menuAnts;
	menuAnts.loadFromFile("media/menu.png");

	sf::Sprite renderMenu(menuAnts);
	renderMenu.setPosition(50, 300);
	p_renderWindow->draw(renderMenu);

	sf::Text antInfoTest;
	antInfoTest.setFont(p_guiFont);
	antInfoTest.setString("Red Ant: (Ranged) Average Movement/High Damage \nBlue Ant: (Melee) Slow Movement/High Damage \nBlack Ant: (Melee) Fast Movement/Low Damage \n ");
	antInfoTest.setCharacterSize(14);
	antInfoTest.setPosition(275, 310);
	p_renderWindow->draw(antInfoTest);

	sf::Text startText;
	startText.setFont(p_guiFont);
	startText.setString("Prese enter to start");
	startText.setCharacterSize(30);
	startText.setPosition(185, 400);
	p_renderWindow->draw(startText);

	p_renderWindow->display();
}