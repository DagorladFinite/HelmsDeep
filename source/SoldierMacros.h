#pragma once

#include "StateMachine.h"
#include "Boid.h"

#define STATE_IDLE 0
#define STATE_PROTECT 1
#define STATE_ATTACK 2

struct SoldierMacros : public Boid, public StateMachine
{
	virtual bool States(StateMachineEvent event, int state) override;
};

