#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Time.hpp>

#include <vector>

class Animation
{
public:
	typedef std::vector<sf::IntRect> Frames;

public:
	Animation(int start, int end, int count, int width, int height, sf::Time frameTime, int type);
	~Animation() {}

	sf::IntRect getFrame() const;

	void update(sf::Time elapsed);

	bool isRunning() const;
	void start();
	void stop();

private:
	void nextFrame();
	void previousFrame();

private:
	int runType_;
	bool running_;

	Frames frames_;
	int currentFrame_;
	int frameCount_;

	sf::Time frameTime_;
	sf::Time lastTime_;
};