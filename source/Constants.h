/* ========================================================================
   File: Constants.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#define DEBUG 1

// Convenience for text handling
#define LINE_HEIGHT 40.0f

// Constants
static const int FIXED_WIDTH = 1024;
static const int FIXED_HEIGHT = 720;

// States
enum StateCode
{
	CONTINUE, QUIT, MENU, FSM_PLAYGROUND
};

