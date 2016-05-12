#include "SoldierMacros.h"


#include "FSMMacros.h"
#include <stdio.h>

bool SoldierMacros::States(StateMachineEvent event, int state)
{
	BeginStateMachine
		State(STATE_IDLE)
		OnEnter

		OnUpdate
		// If miner is not fatigued start to dig for nuggets again.
		if (foodFound)
		{
			ChangeState(STATE_PROTECT);

		}
		else
		{
		}
	OnExit
		State(STATE_PROTECT)
		OnEnter
		SetBehaviour(Behaviour::FLOCKING_SEEK);

		OnUpdate
			if (inDanger) {

				ChangeState(STATE_ATTACK);
			}
		OnExit

			State(STATE_ATTACK)
			OnEnter
			// On entry the miner makes sure he is located at the bank
			SetBehaviour(Behaviour::SEEK_KINEMATIC);

	OnUpdate
		if (!inDanger) {

			ChangeState(STATE_PROTECT);

		}
	OnExit
		EndStateMachine
}