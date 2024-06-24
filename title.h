#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************




//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);

void IdleText(void);
void LogoText1(void);
void LogoText2(void);
void LogoText3(void);
void MenuText1(void);
void DrawOption(void);

int* Get_g_Cursor(void);
void DrawVolumeMas(void);
void DrawVolumeBGM(void);
void DrawVolumeSE(void);

void DrawOptionSelect(void);
void MenuText2(void);
void DrawEnter(void);
void DrawOptionKey(void);
void UpdateOption(void);

void SetPhase(void);
BOOL* GetGReturn(void);
BOOL* GetReturn2(void);