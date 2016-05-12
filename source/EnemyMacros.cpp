#include "EnemyMacros.h"

#include "FSMMacros.h"
#include <stdio.h>

bool EnemyMacros::States(StateMachineEvent event, int state)
{
	BeginStateMachine
		State(STATE_IDLE)
		OnEnter

		OnUpdate
		// If miner is not fatigued start to dig for nuggets again.
		if (foodFound)
		{
			ChangeState(STATE_WANDER);

		}
	OnExit
		State(STATE_WANDER)
		OnEnter
		SetBehaviour(Behaviour::SIMPLE_WANDER);
	OnUpdate

		if (hurt) {

			ChangeState(STATE_RETREAT);

		}
	OnExit
		State(STATE_RETREAT)
		OnEnter
		SetBehaviour(Behaviour::ARRIVE);
	OnUpdate


		if (!hurt) {
			ChangeState(STATE_WANDER);
		}
	OnExit

		EndStateMachine
}
