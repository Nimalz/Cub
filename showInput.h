#pragma once
#include "main.h"

struct InputDetect
{
	BOOL T_ESC;
	BOOL T_ENTER;

	BOOL T_UP;
	BOOL T_DOWN;
	BOOL T_LEFT;
	BOOL T_RIGHT;

	BOOL T_ABI1;
	BOOL T_ABI2;

	BOOL T_RESET;

};

struct InputCounter
{
	int C_ESC;
	int C_ENTER;
		
	int C_UP;
	int C_DOWN;
	int C_LEFT;
	int C_RIGHT;
		
	int C_ABI1;
	int C_ABI2;
		
	int C_RESET;

};

void InitSI(void);
void UninitSI(void);
void UpdateSI();
void DrawSI(void);

BOOL* GetShowSI(void);
InputDetect* GetInputStates(void);
void DrawSIText(void);