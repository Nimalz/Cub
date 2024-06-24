#pragma once
#include "sound.h"
#define TEST

#ifdef TEST
#define STAGENUM                    (4)
#else
#define STAGENUM                    (STAGE_MAX)
#endif
#define MAP_SIZE_WIDTH				(8)		        // MAP�̉���(�l�̓`�b�v��)
#define MAP_SIZE_HEIGHT				(8)		        // MAP�̏c��(�l�̓`�b�v��)

#define MAPCHIP_WIDTH               (160)			// �}�b�v�`�b�v�̉���
#define MAPCHIP_HEIGHT              (160)		    // �}�b�v�`�b�v�̏c��
#define MAPCHIP_POLI_WID            (169)		    // �e�N�X�`���ɍ��킹��|���S���̉���
#define MAPCHIP_POLI_HEI            (169)		    // �e�N�X�`���ɍ��킹��|���S���̏c��
#define MAP_SIZE_MOD                (0.8f)		    // �}�b�v�T�C�Y�����p

#define MAPCHIP_DISP				(MAPCHIP_WIDTH*MAP_SIZE_MOD)
#define POLI_DISP					(MAPCHIP_POLI_WID*MAP_SIZE_MOD)

#define DEFAULT_X					(SCREEN_CENTER_X)
#define DEFAULT_Y					(150.0f)

#define UNPASS                      (0.0f)					//�󂫃u���b�N����
#define STAIRLR                     (0.4f)					//���E�K�i
#define STAIRUD                     (0.5f)					//�㉺�K�i

enum STAGE
{
	STAGE_1,
	STAGE_2,
	STAGE_3,
	STAGE_4,
	STAGE_5,
	STAGE_6,

	STAGE_MAX
};

enum BlockInput
{
	B_IDLE,
	B_UP,
	B_DOWN,
	B_LEFT,
	B_RIGHT,

	B_MAX,
};

struct STAGEINFO
{
	int stage;	//�X�e�[�W
	int px, py;		//�v���C���[�����ʒu
	int gx, gy;		//�S�[���ʒu
	MUSIC bgm;
};

HRESULT InitField(void);
void UninitField(void);
void UpdateField(void);
void DrawField(void);

int* GetStage(void);
float* GetMC(void);

void DrawFlag(int x, int y);
void Drawrim(int x, int y);
void DrawHover(int x, int y);
void RimCheck(int x, int y);
float PassCheck(int depth, int width, int height, float* MappChipList, int z, int x, int y);
void BlockMoveInput(void);
BOOL GetSelect(void);
void DrawArrowSelect(void);
int UpdateBlock(void);
void LoadStage(void);
void DrawBlockMovePreview(void);
//void DrawBlockMove(void);
void WriteOutMap(int);
void WriteInMap(int stepnum);
STAGEINFO* GetStageInfo(void);
BOOL ClearCheck(void);
void DrawClear(void);
void DrawShiftOut(void);
void DrawShiftIn(void);
int* GetStageClear(void);
void DrawFrame(void);
void DrawUI(void);
BOOL GetMenuFlag(void);
BOOL GetBlockInput(void);
int GetMenuCursor(void);
void DrawPlayer();
