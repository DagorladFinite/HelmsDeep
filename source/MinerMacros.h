/* ========================================================================
   File: MinerMacros.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "StateMachine.h"
#include "Miner.h"

#define STATE_IDLE 0
#define STATE_GATHER 1
#define STATE_FLEE 2

struct MinerMacros : public Miner, public StateMachine
{
	virtual bool States(StateMachineEvent event, int state) override;
};