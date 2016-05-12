/* ========================================================================
   File: DecisionTreeInteractive.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "DecisionTreeInteractive.h"

bool InteractiveDecision::GetBranch()
{
	if (decisionResult == TRUE_NODE)
	{
		return true;
	}
	else
	{
		return false;
	}
}

DecisionTreeNode* InteractiveDecision::MakeDecision()
{
	if (decisionResult == PENDING)
	{
		return nullptr;
	}

	// Choose a branch based on the GetBranch method
	if (GetBranch())
	{
		// Make sure its not null before recursing.
		if (trueBranch == nullptr)
		{
			return nullptr;
		}
		else
		{
			return trueBranch;
		}
	}
	else {
		// Make sure its not null before recursing.
		if (falseBranch == nullptr)
		{
			return nullptr;
		}
		else
		{
			return falseBranch;
		}
	}
}

DecisionTreeNode* InteractiveDecision::Update()
{
	if (decisionResult != PENDING)
	{
		return MakeDecision();
	}
	else
	{
		return nullptr;
	}
}

void InteractiveDecision::HandleInput()
{
	if (InputSingleton::Instance()->IsKeyDown(SDL_SCANCODE_Y))
	{
		decisionResult = TRUE_NODE;
	}
	else if (InputSingleton::Instance()->IsKeyDown(SDL_SCANCODE_N))
	{
		decisionResult = FALSE_NODE;
	}
}
