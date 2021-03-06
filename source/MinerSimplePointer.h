/* ========================================================================
   File: MinerSimplePointer.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma  once

#include "State.h"
#include "Miner.h"

struct MinerSimplePointer : public Miner
{
	State* currentState;

	virtual void InitMiner(char* name) override;
	virtual void Update() override;

	void ChangeState(State* new_state);
};

