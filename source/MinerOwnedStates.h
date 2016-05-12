/* ========================================================================
File: MinerOwnedStates.h
Revision: 0.1
Creator: David Collado Ochoa
Notice: Code inspired by Mat Buckland 2002 (fup@ai-junkie.com)
======================================================================== */

#pragma once

#include "State.h"
struct MinerSimplePointer;

//------------------------------------------------------------------------
//
//  In this state the miner will walk to a goldmine and pick up a nugget
//  of gold. If the miner already has a nugget of gold he'll change state
//  to VisitBankAndDepositGold. If he gets thirsty he'll change state
//  to QuenchThirst
//------------------------------------------------------------------------
struct EnterMineAndDigForNugget : public State
{
	// This is a singleton
	static EnterMineAndDigForNugget* Instance();
	virtual void Enter(MinerSimplePointer* miner);
	virtual void Update(MinerSimplePointer* miner);
	virtual void Exit(MinerSimplePointer* miner);
};

//------------------------------------------------------------------------
//
//  Entity will go to a bank and deposit any nuggets he is carrying. If the 
//  miner is subsequently wealthy enough he'll walk home, otherwise he'll
//  keep going to get more gold
//------------------------------------------------------------------------
struct VisitBankAndDepositGold : public State
{
	// This is a singleton
	static VisitBankAndDepositGold* Instance();
	virtual void Enter(MinerSimplePointer* miner);
	virtual void Update(MinerSimplePointer* miner);
	virtual void Exit(MinerSimplePointer* miner);
};

//------------------------------------------------------------------------
//
//  Miner will go home and sleep until his fatigue is decreased
//  sufficiently
//------------------------------------------------------------------------
struct GoHomeAndSleepTilRested : public State
{
	// This is a singleton
	static GoHomeAndSleepTilRested* Instance();
	virtual void Enter(MinerSimplePointer* miner);
	virtual void Update(MinerSimplePointer* miner);
	virtual void Exit(MinerSimplePointer* miner);
};

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
struct QuenchThirst : public State
{
	// This is a singleton
	static QuenchThirst* Instance();
	virtual void Enter(MinerSimplePointer* miner);
	virtual void Update(MinerSimplePointer* miner);
	virtual void Exit(MinerSimplePointer* miner);
};
