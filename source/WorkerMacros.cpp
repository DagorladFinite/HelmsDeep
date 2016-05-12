#include "WorkerMacros.h"

#include "FSMMacros.h"
#include <stdio.h>

bool WorkerMacros::States(StateMachineEvent event, int state)
{
	BeginStateMachine
		State(STATE_IDLE)
		OnEnter

		//Cuando se entra en este estado, el Uruk pasa automáticamente a estar en idle
		SetBehaviour(Behaviour::NONE);
	OnUpdate
		
		//En el primer update de la máquina de estados, comprobamos si la partida ha iniciado y ponemos en marcha a nuestro Uruk
		//El estado idle es un estado "safe" al que se puede volver siempre que pase algo extraño
		
		if (started) {
			ChangeState(STATE_SEEK);
		}
			
		
	OnExit

		State(STATE_SEEK)
		OnEnter
		//Cuando entren en combat, los Uruks determinarán cual es el objetivo prioritario al que atacar
		SetBehaviour(Behaviour::ARRIVE);
	OnUpdate
		if (position.Distance(position, *targetPosition))
	{

		ChangeState(STATE_COMBAT);

	}
	OnExit


	State(STATE_COMBAT)
		OnEnter
		SetBehaviour(Behaviour::NONE);
		
	OnUpdate

	OnExit

		State(STATE_NAVIGATE)
		OnEnter

	OnUpdate

	OnExit
	EndStateMachine
}
