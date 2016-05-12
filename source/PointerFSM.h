/* ========================================================================
   File: PointerFSM.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

// Local Includes
#include "PointerState.h"

// FSM Class
class PointerFSM
{
protected:
	PointerState *currentState;						// Current State

public:
	// Constructor
	PointerFSM() {}
	// Destructor
	virtual ~PointerFSM() {}

	// Global Functions
	virtual void Update();

	// State Functions
	bool IsState(PointerState &state);
	void ChangeState(PointerState &newState);
};

