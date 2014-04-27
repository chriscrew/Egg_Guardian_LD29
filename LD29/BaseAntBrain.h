#pragma once

class BaseAnt;

class BaseAntBrain
{
public:
	BaseAntBrain(BaseAnt* owner)
	{
		p_owner = owner;
	}

	virtual void think() = 0;

protected:
	BaseAnt* p_owner;
};