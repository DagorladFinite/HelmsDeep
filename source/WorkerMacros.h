#pragma once

#include "StateMachine.h"
#include "Boid.h"

#define STATE_IDLE 0
#define STATE_SEEK 1
#define STATE_COMBAT 2
#define STATE_NAVIGATE 3

struct WorkerMacros : public Boid, public StateMachine
{
	bool wallDestroyed;
	bool gateOpen;
	bool started;
	virtual bool States(StateMachineEvent event, int state) override;
};

