/* ========================================================================
   File: DecisionTreeState.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "FSM_Playground.h"
#include <time.h>

void FSM_Playground::Init()
{
	// Initialize Fonts of different sizes
	SetResourcesFilePath("Fonts/nokiafc22.ttf");
	fontNormal = FC_CreateFont();
	FC_LoadFont(fontNormal, display->renderer, resourcesFilePath, 20,
		FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
	fontSmall = FC_CreateFont();
	FC_LoadFont(fontSmall, display->renderer, resourcesFilePath, 22,
		FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
	fontBig = FC_CreateFont();
	FC_LoadFont(fontBig, display->renderer, resourcesFilePath, 32,
		FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);

	boidTexture = {};
	SetResourcesFilePath("Images/Boid.png");
	if (Texture_LoadFromFile(&boidTexture, display->renderer, resourcesFilePath) == false)
	{
		SDL_Log("Failed to load Image!\n");
	}

	playerTexture = {};
	SetResourcesFilePath("Images/Player.png");
	if (Texture_LoadFromFile(&playerTexture, display->renderer, resourcesFilePath) == false)
	{
		SDL_Log("Failed to load Image!\n");
	}

	playerAttacking = {};
	SetResourcesFilePath("Images/PlayerAttacking.png");
	if (Texture_LoadFromFile(&playerAttacking, display->renderer, resourcesFilePath) == false)
	{
		SDL_Log("Failed to load Image!\n");
	}

	srand(time(NULL));

	Path_AddPoint(&recorrido, { 400,500 });
	Path_AddPoint(&recorrido, { 600,500 });

	test.position.x = 0;
	test.position.y = 0;

	test.SetBehaviour(Behaviour::PATH_FOLLOWING);

	test.path = &recorrido;
	test.loopPath = false;

	test.targetPosition = &archer.position;

	test.slowingRadius = 25;

	archer.position.x = 750;
	archer.position.y = 500;

	boomEntryArea.center.x = 400;
	boomEntryArea.center.y = 500;
	boomEntryArea.radius = 50;

	boomExitArea.center.x = 600;
	boomExitArea.center.y = 500;
	boomExitArea.radius = 50;
	
}

void FSM_Playground::Deinit()
{
	FC_FreeFont(fontNormal);
	FC_FreeFont(fontSmall);
	FC_FreeFont(fontBig);
}

StateCode FSM_Playground::HandleInput()
{

	haldir.HandleInput();
	if (InputSingleton::Instance()->doQuit)
	{
		return StateCode::QUIT;
	}
	// keyboard Input Handling
	else if (InputSingleton::Instance()->IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		return StateCode::MENU;
	}
	else if (InputSingleton::Instance()->IsKeyDown(SDL_SCANCODE_SPACE))
	{
		paused = !paused;
	}
	else if (InputSingleton::Instance()->IsKeyDown(SDL_SCANCODE_N))
	{
		if (paused == false)
		{
			paused = true;
		}
		else
		{
			runOneFrame = true;
		}
	}

	return StateCode::CONTINUE;
}

void FSM_Playground::Update(float deltaTime)
{
	if (paused == true && runOneFrame == false)
	{
		return;
	}
	runOneFrame = false;

	// Update Entities
	test.Update(deltaTime);
	haldir.Update(deltaTime);
	
	//Los orkis pegan
	if (test.position.Distance(test.position, archer.position) <= 25) {

		if (archer.alive && test.alive) {
			archer.hp = archer.hp - test.atk;

		}


		if (archer.hp <= 0) {
			archer.alive = false;
		}

	}

	//Haldir pega
	if (haldir.position.Distance(haldir.position,test.position) <= 25) {

		if (test.alive) {
			test.hp = test.hp - 2;

		}


		if (test.hp <= 0) {
			test.alive = false;
		}

	}
}

void FSM_Playground::Render()
{
	if (paused == true)
	{
		FC_DrawAlign(fontBig, display->renderer,
			0,
			FIXED_HEIGHT - LINE_HEIGHT,
			FC_ALIGN_LEFT,
			(char*)"Pause");
	}
	else
	{
		FC_DrawAlign(fontBig, display->renderer,
			0,
			FIXED_HEIGHT - LINE_HEIGHT,
			FC_ALIGN_LEFT,
			(char*)"Running");
	}
	Area_Render(&boomEntryArea, display->renderer, Colors::ASBESTOS);
	Area_Render(&boomExitArea, display->renderer, Colors::ASBESTOS);
	
	if (test.alive) {
		test.Render(&boidTexture, display->renderer);
	}


	haldir.Render(&playerTexture, display->renderer);

	if (archer.alive) {
		archer.Render(&playerTexture, display->renderer);
	}
	
}
