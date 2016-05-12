/* ========================================================================
   File: CleanerRobotEnergy.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma  once

#include "PointerFSM.h"
#include "CleanerRobotClean.h"

const int K_MAX_ENERGY = 5;

struct CleanerRobotEnergy : public PointerFSM
{
	PointerStateTemplate<CleanerRobotEnergy> stateCleanup;
	PointerStateTemplate<CleanerRobotEnergy> stateCharge;

	CleanerRobotEnergy();

	CleanerRobotClean cleanFSM;
	CleanerLocationType currentLocation = clean;
	char name[48];
	char stateInformation[256];
	char currentStateInformation[24];

	int energy = K_MAX_ENERGY;
	virtual void InitCleanerRobotEnergy(char* name);
	virtual void Update() override;

	CleanerLocationType Location()
	{
		return currentLocation;
	}
	void ChangeLocation(CleanerLocationType loc)
	{
		currentLocation = loc;
	}

	// Methods for StateClean
	void CleanerRobotEnergy::StateCleanup_Enter();
	void CleanerRobotEnergy::StateCleanup_Update();
	void CleanerRobotEnergy::StateCleanup_Exit();

	// Methods for StateCharge
	void CleanerRobotEnergy::StateCharge_Enter();
	void CleanerRobotEnergy::StateCharge_Update();
	void CleanerRobotEnergy::StateCharge_Exit();
};

