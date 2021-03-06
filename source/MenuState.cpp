/* ========================================================================
   File: MenuState.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "MenuState.h"

void MenuState::Init()
{
	// Initialize Font
	SetResourcesFilePath("Fonts/nokiafc22.ttf");
	fontSmall = FC_CreateFont();
	FC_LoadFont(fontSmall, display->renderer, resourcesFilePath, 30, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
	fontBig = FC_CreateFont();
	FC_LoadFont(fontBig, display->renderer, resourcesFilePath, 40, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
}

void MenuState::Deinit()
{
	FC_FreeFont(fontBig);
}

StateCode MenuState::HandleInput()
{
	if (InputSingleton::Instance()->IsKeyDown(SDL_SCANCODE_ESCAPE) 
		|| InputSingleton::Instance()->doQuit)
	{
		return StateCode::QUIT;
	}
	else if (InputSingleton::Instance()->IsKeyDown(SDL_SCANCODE_SPACE))
	{
		return StateCode::FSM_PLAYGROUND;
	}

	return StateCode::CONTINUE;
}

void MenuState::Update(float deltaTime)
{
	drawPromptCounter += deltaTime;
	if (drawPromptCounter > K_DRAW_PROMPT_THRESHOLD)
	{
		drawPrompt = !drawPrompt;
		drawPromptCounter = 0.0f;
	}
}

void MenuState::Render()
{
	// Render Text
	FC_DrawAlign(fontSmall, display->renderer,
		0,
		0,
		FC_ALIGN_LEFT,
		(char*)"Final Project");

	FC_DrawAlign(fontBig, display->renderer,
		FIXED_WIDTH / 2,
		FIXED_HEIGHT / 2 - 100,
		FC_ALIGN_CENTER,
		(char*)"Helm's Deep");

	if (drawPrompt == true)
	{
		FC_DrawAlign(fontSmall, display->renderer,
			FIXED_WIDTH / 2,
			FIXED_HEIGHT / 2 + 100,
			FC_ALIGN_CENTER,
			(char*)"Press SPACE to become a homosex Rohan warrior");
	}
}


