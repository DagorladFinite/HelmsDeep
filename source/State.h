/* ========================================================================
File: State.h
Revision: 0.1
Creator: David Collado Ochoa
Notice: Code inspired by Mat Buckland 2002 (fup@ai-junkie.com)
======================================================================== */

#pragma once

struct MinerSimplePointer;

struct State
{
	// This will execute when the state is entered
	virtual void Enter(MinerSimplePointer*) = 0;
	// This is the state's normal update function
	virtual void Update(MinerSimplePointer*) = 0;
	// This will execute when the state is exited.
	virtual void Exit(MinerSimplePointer*) = 0;
};
