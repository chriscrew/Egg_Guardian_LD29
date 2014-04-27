#pragma once

#include "BaseAntBrain.h"

class MeleeAntBrain : public BaseAntBrain
{
public:
	MeleeAntBrain(BaseAnt* owner);

	virtual void think();
};