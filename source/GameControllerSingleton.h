/* ========================================================================
   File: GameControllerSingleton.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Common/GameFramework.h"
#include "Player.h"
#include "Area.h"

struct GameControllerSingleton
{
	Player* player;
	bool playerAttacking = false;
	Area* healthArea;
	Area* coverArea;

	static GameControllerSingleton& Instance()
	{
		static GameControllerSingleton singleton;
		return singleton;
	}
};