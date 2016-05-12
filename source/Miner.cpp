/* ========================================================================
File: FSM_VariantState.h
Revision: 0.1
Creator: David Collado Ochoa
Notice: Code inspired by Mat Buckland 2002 (fup@ai-junkie.com)
======================================================================== */

#include "Miner.h"
#include <string.h> 

void Miner::InitMiner(char* name)
{
	strcpy(this->name, name);
	currentLocation = home;
	goldCarried = 0;
	moneyInBank = 0;
	thirst = 0;
	fatigue = 0;
}

void Miner::AddToGoldCarried(int val)
{
	goldCarried += val;
	if (goldCarried < 0) 
	{
		goldCarried = 0;
	}
}

void Miner::AddToWealth(int val)
{
	moneyInBank += val;
	if (moneyInBank < 0)
	{
		moneyInBank = 0;
	}
}

bool Miner::Thirsty()
{
	if (thirst >= K_THIRST_LEVEL) 
	{ 
		return true; 
	}

	return false;
}

void Miner::Update()
{
	thirst += 1;
}

bool Miner::Fatigued()
{
	if (fatigue > K_TIREDNESS_THRESHOLD)
	{
		return true;
	}
	return false;
}