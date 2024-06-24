#pragma once


HRESULT InitOption(void);
void UninitOption(void);
void UpdateOption(void);
void DrawOption(void);

void DrawOptionKey(void);
void DrawOptionSelect(void);
void DrawVolumeMas(void);
void DrawVolumeBGM(void);
void DrawVolumeSE(void);
BOOL* GetOptionFlag(void);
void DrawOptions(void);
int* GetCursorOption(void);
