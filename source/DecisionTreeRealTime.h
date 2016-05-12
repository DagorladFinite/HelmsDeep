/* ========================================================================
   File: DecisionTreeRealTime.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Common/GameFramework.h"
#include "DecisionTree.h"

// Real Time Decision Tree

typedef bool(*DecisionFunction)();
typedef void(*ActionFunction)();

struct RealTimeDecision : public Decision
{
	char* decisionText; // For debug purposes
	DecisionFunction decisionFunction;

	void CreateRealTimeDecision(DecisionFunction decisionFunction, char* decisionText);
	virtual bool GetBranch() override;
};

struct RealTimeAction : public DecisionTreeAction
{
	char* actionText; // For debug purposes
	ActionFunction actionFunction;

	void CreateRealTimeAction(ActionFunction actionFunction, char* actionText);
	virtual DecisionTreeNode* MakeDecision();
};

