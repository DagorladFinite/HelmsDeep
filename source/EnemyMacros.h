#pragma once

#include "StateMachine.h"
#include "Boid.h"

#define STATE_IDLE 0
#define STATE_WANDER 1
#define STATE_RETREAT 2

struct EnemyMacros : public Boid, public StateMachine
{
	virtual bool States(StateMachineEvent event, int state) override;
};
