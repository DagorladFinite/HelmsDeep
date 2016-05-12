/* ========================================================================
   File: PointerState.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

// CState Class
class PointerState
{
public:
	// Destructor
	virtual ~PointerState() {}

	// State Functions
	virtual void ExecuteEnter() = 0;
	virtual void ExecuteUpdate() = 0;
	virtual void ExecuteExit() = 0;
};

// CStateTemplate Class
template <class T>
class PointerStateTemplate : public PointerState
{
protected:
	typedef void (T::*POINTER_FUNCTION_STATE)();
	T* instance; // Instance Pointer
	POINTER_FUNCTION_STATE functionEnterState; // State Function Pointer
	POINTER_FUNCTION_STATE functionUpdateState; // State Function Pointer
	POINTER_FUNCTION_STATE functionExitState; // State Function Pointer

public:
	// Constructor
	PointerStateTemplate() : instance(0), functionEnterState(0), functionUpdateState(0), functionExitState(0) {}

	// Initialize Functions
	void Set(T *instance,
		POINTER_FUNCTION_STATE functionEnterState,
		POINTER_FUNCTION_STATE functionUpdateState,
		POINTER_FUNCTION_STATE functionExitState)
	{
		// Set Instance
		this->instance=instance;
		// Set Function Pointers
		this->functionEnterState = functionEnterState;
		this->functionUpdateState = functionUpdateState;
		this->functionExitState = functionExitState;
	}

	// State Functions
	virtual void ExecuteEnter()
	{
		// Begin State
		(instance->*functionEnterState)();
	}
	virtual void ExecuteUpdate()
	{
		// State
		(instance->*functionUpdateState)();
	}
	virtual void ExecuteExit()
	{
		// End State
		(instance->*functionExitState)();
	}
};

