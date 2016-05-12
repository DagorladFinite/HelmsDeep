/* ========================================================================
   File: DecisionTreeInteractive.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Common/GameFramework.h"
#include "DecisionTree.h"

enum DecisionResultEnum
{
	PENDING, TRUE_NODE, FALSE_NODE
};

struct InteractiveDecision : public Decision
{
	char *decisionText;
	DecisionResultEnum decisionResult = PENDING;

	virtual bool GetBranch();
	virtual DecisionTreeNode* MakeDecision();

	void HandleInput();
	DecisionTreeNode* Update();
};

struct InteractiveAction : public DecisionTreeAction
{
	char *actionText;
};


