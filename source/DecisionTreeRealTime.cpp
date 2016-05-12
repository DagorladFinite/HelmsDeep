/* ========================================================================
   File: DecisionTreeRealTime.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "DecisionTreeRealTime.h"
  
// RealtimeDecision

void RealTimeDecision::CreateRealTimeDecision(DecisionFunction decisionFunction, char* decisionText)
{
	this->type = DECISION;
	this->decisionFunction = decisionFunction;
	this->decisionText = decisionText;
}

bool RealTimeDecision::GetBranch()
{
	SDL_Log("Executing Decision Function for decision %s", decisionText);
	return decisionFunction();
}

// RealtimeAction

void RealTimeAction::CreateRealTimeAction(ActionFunction actionFunction, char* actionText)
{
	this->type = ACTION;
	this->actionFunction = actionFunction;
	this->actionText = actionText;
}

DecisionTreeNode* RealTimeAction::MakeDecision()
{
	SDL_Log("Executing Make Decision Function for action %s", actionText);
	actionFunction();
	return this;
}
