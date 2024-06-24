
#pragma once

#include <windows.h>
#include "xaudio2.h"						// サウンド処理で必要

struct VOLUME
{
	float master;
	float BGM;
	float SE;
};
//*****************************************************************************
// サウンドファイル
//*****************************************************************************
enum MUSIC
{
	SOUND_LABEL_BGM_sample000,	// BGM0
	SOUND_LABEL_BGM_sample001,	// BGM1
	SOUND_LABEL_BGM_sample002,	// BGM2
	SOUND_LABEL_BGM_result,
	SOUND_LABEL_SE_menu1,		// MENU操作音１
	SOUND_LABEL_SE_confirm,	// 
	SOUND_LABEL_SE_restart,	// 
	SOUND_LABEL_SE_rim_scan,		// 
	SOUND_LABEL_SE_Deny,	// 
	SOUND_LABEL_SE_move_confirm,	// 
	SOUND_LABEL_SE_move_select,		// 
	SOUND_LABEL_SE_cancel,		// 


	SOUND_LABEL_MAX,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(int label);
void StopSound(int label);
void StopSound(void);
void PauseSound(int label);
void UnpauseSound(int label);
BOOL GetPaused(void);
void Sampleratedown(int label,float mul);

IXAudio2SourceVoice* GetSourceVoice(int index);
VOLUME* GetVolume(void);
void SetVolume(void);
