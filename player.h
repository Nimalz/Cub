
#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER_MAX			(1)		// �v���C���[��Max�l��

#define	PLAYER_OFFSET_CNT	(16)	// 16���g

// �v���C���[�̉�ʓ��z�u���W
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
// �\���̒�`
//*****************************************************************************


struct PLAYER
{
	SPRITE		sprite;			// ���W�ƃe�N�X�`�����
	INPUTMODE	inputmode;		// ���샂�[�h
	float		speed;			// ���x
	BOOL		moving;         // �ړ����t���O
	BOOL        use;
	//float		mass;			// ����
	//XMFLOAT3	acc;			// �����x
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
// �v���g�^�C�v�錾
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

