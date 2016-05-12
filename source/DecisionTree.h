/* ========================================================================
   File: DecisionTree.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Common/GameFramework.h"

enum NodeTypeEnum
{
	DECISION, ACTION
};

struct DecisionTreeNode
{
	NodeTypeEnum type;
	void InitDecision()
	{
		type = DECISION;
	}
	virtual DecisionTreeNode* MakeDecision() = 0;
};

struct Decision : public DecisionTreeNode
{
	DecisionTreeNode* trueBranch;
	DecisionTreeNode* falseBranch;

	virtual bool GetBranch() = 0;
	virtual DecisionTreeNode* MakeDecision() override;
};

struct DecisionTreeAction : public DecisionTreeNode
{
	void InitAction()
	{
		type = ACTION;
	}
	virtual DecisionTreeNode* MakeDecision() override;
};


