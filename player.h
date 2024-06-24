
#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_MAX			(1)		// プレイヤーのMax人数

#define	PLAYER_OFFSET_CNT	(16)	// 16分身

// プレイヤーの画面内配置座標
#define PLAYER_DISP_X				(SCREEN_WIDTH/2)
#define PLAYER_DISP_Y				(SCREEN_HEIGHT/2 + TEXTURE_HEIGHT)

enum INPUTMODE
{
	INPUT_TITLE,
	INPUT_TUIT,
	INPUT_GAME,
	INPUT_PAUSE,

	INPUT_MAX
};

//*****************************************************************************
// 構造体定義
//*****************************************************************************


struct PLAYER
{
	SPRITE		sprite;			// 座標とテクスチャ情報
	INPUTMODE	inputmode;		// 操作モード
	float		speed;			// 速度
	BOOL		moving;         // 移動中フラグ
	BOOL        use;
	//float		mass;			// 質量
	//XMFLOAT3	acc;			// 加速度
};

enum FACING
{
	F_IDLE = 0,

	F_UP = 1,
	F_RIGHT = 1 << 1,
	F_DOWN = 1 << 2,
	F_LEFT = 1 << 3,

	F_RD = F_RIGHT ^ F_DOWN,

	F_LD = F_LEFT ^ F_DOWN,

	F_LU = F_LEFT ^ F_UP,
	F_RU = F_RIGHT ^ F_UP,

	F_MAX
};

struct StairStats
{
	float	   x;
	float      y;
};

struct Stair;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
//void DrawPlayer(void);

void UpdatePre();
//void DrawPre();

PLAYER* GetPlayer(void);
int GetStair(void);
StairStats* GetStairBlock(void);
void MoveControl(int depth, int width, int height, float* MappChipList, PLAYER* player, XMFLOAT3 old_pos);
void ClearStair(void);

