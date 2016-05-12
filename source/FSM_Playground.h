/* ========================================================================
   File: DecisionTreeState.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Common/GameFramework.h"
#include "Boid.h"
#include "WorkerMacros.h"
#include "SoldierMacros.h"
#include "EnemyMacros.h"
#include "Area.h"
#include "Path.h"
#include "Player.h"

class FSM_Playground : public BaseState
{
public:
	virtual void Init() override;
	virtual void Deinit() override;
	virtual StateCode HandleInput() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;

	// State Data
	bool stateFoodFound = false;
	int flockNumber = 5;

	// Textures
	Texture boidTexture;
	Texture playerTexture;
	Texture playerAttacking;
	
	WorkerMacros test;
	Boid archer;
	Player haldir;
	
	Path recorrido;

	Area boomEntryArea;
	Area boomExitArea;

	// Debug Data and Font
	bool runOneFrame = false;
	FC_Font* fontBig;
	FC_Font* fontNormal;
	FC_Font* fontSmall;
};