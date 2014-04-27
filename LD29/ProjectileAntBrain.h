#pragma once

#include "BaseAntBrain.h"

class ProjectileAntBrain : public BaseAntBrain
{
public:
	ProjectileAntBrain(BaseAnt* owner);

	virtual void think();
};