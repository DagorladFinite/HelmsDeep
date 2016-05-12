#include "WorkerMacros.h"

#include "FSMMacros.h"
#include <stdio.h>

bool WorkerMacros::States(StateMachineEvent event, int state)
{
	BeginStateMachine
		State(STATE_IDLE)
		OnEnter

	OnUpdate
		// If miner is not fatigued start to dig for nuggets again.
		if (foodFound)
		{
			ChangeState(STATE_GATHER);
			
		}
		else
		{
			SetBehaviour(Behaviour::NONE);
		}
	OnExit
		State(STATE_GATHER)
		OnEnter
		SetBehaviour(Behaviour::SEEK_STEERING);
		//TARGET DEL SEEK
		//targetPosition
	OnUpdate
	if(inDanger)
	{
		ChangeState(STATE_FLEE);

	}
	OnExit
	State(STATE_FLEE)
		OnEnter
		SetBehaviour(Behaviour::FLEE_STEERING);
		//targets[1]
	OnUpdate

		if (!inDanger) {

			ChangeState(STATE_GATHER);
		}
	OnExit
	EndStateMachine
}
