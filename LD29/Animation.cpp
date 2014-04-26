#include "stdafx.h"
#include "Animation.h"

Animation::Animation(int start, int end, int count, int width, int height, sf::Time frameTime, int type)
: currentFrame_(0), frameCount_(count), frameTime_(frameTime), lastTime_(sf::Time::Zero), runType_(type), running_(false), frames_(count)
{
	for (int i = 0; i < frameCount_; i++)
	{
		int tileNum = start + i;
		if (start > end) tileNum = start - i;

		int texX = tileNum % (371 / width);
		int texY = tileNum / (371 / width);

		frames_[i] = sf::IntRect(texX * width, texY * height, width, height);
	}
}

bool Animation::isRunning() const
{
	return running_;
}

void Animation::start()
{
	running_ = true;
	currentFrame_ = 0;
}

void Animation::stop()
{
	currentFrame_ = 0;
	running_ = false;
}

sf::IntRect Animation::getFrame() const
{
	return frames_[currentFrame_];
}

void Animation::update(sf::Time elapsed)
{
	lastTime_ += elapsed;

	if (lastTime_ > frameTime_)
	{
		nextFrame();

		if (currentFrame_ == 0 && runType_ == 1)
		{
			previousFrame();
			stop();
		}

		lastTime_ = sf::Time::Zero;
	}
}

void Animation::nextFrame()
{
	int nextFrame = currentFrame_;
	currentFrame_ = (nextFrame + 1) % frames_.size();
}

void Animation::previousFrame()
{
	int prevFrame = currentFrame_;
	currentFrame_ = (prevFrame + frames_.size() - 1) % frames_.size();
}