#pragma once

#include "StateMachine.h"
#include "Boid.h"

#define STATE_IDLE 0
#define STATE_COMBAT 1
#define STATE_NAVIGATE 2

struct WorkerMacros : public Boid, public StateMachine
{

	virtual bool States(StateMachineEvent event, int state) override;
};

