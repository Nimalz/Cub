#include "main.h"
#include "field.h"
#include "sprite.h"
#include "player.h"
#include "bullet.h"
#include "input.h"
#include "collision.h"
#include "file.h"
#include "fade.h"
#include "title.h"
#include "showInput.h"
#include "option.h"
#include "text.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX					(18)				// �e�N�X�`���̐�

#define FLAG_PATTERN_DIVIDE_X		(5)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define FLAG_PATTERN_DIVIDE_Y		(1)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define FLAG_ANIM_PATTERN_NUM		(FLAG_PATTERN_DIVIDE_X*FLAG_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define ANIM_WAIT					(8)		// �A�j���[�V�����̐؂�ւ��Wait�l
//#define ARROW_SIZE					(100)	//�A���[�e�N�X�`����`�悷��T�C�Y
#define BLOCK_BLUE					(13)
#define BLOCK_RED					(14)
#define MENU_OPS					(3)
#define GOAL_SIZE					(1200.0f)			
#define UI_BUTTON_SIZE              (128.0f)
#define TEXTURE_PATTERN_DIVIDE_X	(1)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(3)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;			// ���_���
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static BOOL	 g_Load = FALSE;		// ���������s�������̃t���O(flag)
static int	 g_Stage = 0;			// �X�e�[�W

static BOOL  g_Inputable = TRUE;	// �w�ߗA���H(flag)
static BOOL  g_select = FALSE;		// �n�`���쒆�H(flag)
//static BOOL  g_check = TRUE;		// �n�`����\�H�i�u���b�N���v���C���[������Ă�u���b�N�ƌo�H�ŘA�ڂ��Ă�ƁA����s�\�ɂȂ�j
static int   g_BlockInput = B_IDLE; //�@�u���b�N�����ۑ�
static BOOL  g_InputDone = FALSE;	//�@���͏I�nflag
static int   g_move = 0;			//�@�n�`�ړ��R�}��
static BOOL	 g_clear = FALSE;		// stage clear(flag)
static BOOL  g_shift = FALSE;		// stage clear�A�j���V�����`��(flag)
static BOOL  g_Reboot = FALSE;		// stage �ċN���@flag
static float fx, fy;				//�@�S�[��xy���W
static int   g_Stage_Clear;			// �A�����b�N�����X�e�[�W��
static BOOL  g_First_Hover = 0;		// SE����񂾂��o���悤�ɗp�ӂ���flag
static BOOL  g_draw_frame_flag = FALSE;// �u���b�N���������`�悷�邩�H
static BOOL  g_Menu_Flag = FALSE;		//���j���[�g���Ă�t���O
static int   g_Menu_Cursor = 0;		//���j���[�p�J�[�\��


static float g_Mapnow[MAP_SIZE_HEIGHT][MAP_SIZE_WIDTH];
//�W���W�u���b�N
#ifdef TEST
static float g_MapChipList[STAGENUM][MAP_SIZE_HEIGHT][MAP_SIZE_WIDTH]
{
   {{ 0  , 0  , 1  , 1  , 1  , 1  , 1  , 1},
	{ 0  , 0  , 0  , 1  , 1  , 1  , 1  , 1},
	{ 1  , 1  , 0  , 0  , 1  , 1  , 1  , 1},
	{ 0  , 1  , 1  , 0  , 0  , 1  , 1  , 1},
	{ 0  , 1  , 1  , 1  , 0  , 0  , 1  , 1},
	{ 0  , 1  , 1  , 1  , 1  , 0  , 0  , 1},
	{ 0  , 1  , 1  , 1  , 1  , 1  , 0  , 0},
	{ 0  , 1  , 1  , 1  , 1  , 1  , 1  , 0}},


   {{ 1  , 0  , 0  , 0  , 0  , 0  , 0  , 1},
	{ 0  , 0  , 0  , 0  , 0  , 0  , 0  , 1},
	{ 0  , 0  , 0  , 0  , 0  , 0  , 0  , 1},
	{ 0  , 0  , 0  , 0  , 0  , 0  , 0  , 1},
	{ 1  , 0  , 0  , 0  , 0  , 0  , 1  , 0},
	{ 0  , 0  , 0  , 0  , 0  , 0  , 0  , 1},
	{ 0  , 0  , 0  , 0  , 0  , 0  , 0  , 1},
	{ 0  , 1  , 1  , 1  , 1  , 1  , 0  , 1}},



   {{ 1  , 0  , 0  , 0  , 0  , 0  , 0  , 0},
	{ 1  , 0  , 0  , 0  , 0  , 0  , 0  , 0},
	{ 1  , 0  , 1  , 1  , 0  , 0  , 0  , 0},
	{ 0  , 0  , 1  , 1  , 0  , 0  , 0  , 0},
	{ 1  , 0  , 1  , 1  ,1.21, 0  , 0  , 0},
	{ 1  , 0  , 0  , 0  , 1  , 0  , 0  , 0},
	{ 0  , 0  , 0  , 0  , 0  , 0  , 1.5, 1},
	{ 1  , 1  , 1  , 0  , 0  , 0  , 0  , 1}},

   {{ 0  , 0  , 0  , 0  , 1  , 1  , 1  , 0},
	{ 0  , 0  , 0  , 0  , 1  , 0  , 1  , 0},
	{ 0  , 0  , 0  , 0  , 1  , 0  , 1  , 0},
	{ 0  , 0  , 0  , 1  , 0  , 0  , 0  , 0},
	{ 0  , 1  , 0  , 0  , 1  , 0  , 0  , 0},
	{ 0  , 1  , 0  , 0  , 0  , 0  , 0  , 1.21},
	{ 0  , 1  , 0  , 0  , 0  , 0  , 1.5  , 1},
	{ 1  , 1  , 1  , 0  , 0  , 0  , 1  , 1}},

	/*  {{ 0  , 0  , 0  , 0  , 0  , 0  , 0  , 1},
	   { 1  , 0  , 0  , 1  , 0  , 0  , 0  , 0},
	   { 0  , 0  , 0  , 0  , 0  , 0  , 0  , 1},
	   { 0  , 0  , 0  , 0  , 1.21  , 0  , 0  , 0},
	   { 1  , 0  , 0  , 0  , 1  , 0  , 0  , 1},
	   { 0  , 0  , 1  , 1  , 0  , 0  , 0  , 1},
	   { 0  , 1  , 0  , 0  , 1  , 0  , 0  , 0},
	   { 0  , 0  , 1  , 0  , 0  , 0  ,1.5 , 1}},*/


};
static STAGEINFO g_stageInfo[STAGENUM]
{
	{STAGE_1,5,7,0,0,SOUND_LABEL_BGM_sample001},
	{STAGE_2,7,7,0,0,SOUND_LABEL_BGM_sample001},
	{STAGE_3,7,7,0,0,SOUND_LABEL_BGM_sample001},
	{STAGE_4,4,4,3,3,SOUND_LABEL_BGM_sample001},
	/*{STAGE_5,7,7,0,0,SOUND_LABEL_BGM_sample001},
	{STAGE_6,7,7,0,0,SOUND_LABEL_BGM_sample001},*/
};

#else
static float g_MapChipList[STAGENUM][MAP_SIZE_HEIGHT][MAP_SIZE_WIDTH]
{
   {{ 1  , 1  , 0  , 0  , 1  , 1  , 1  , 1},
	{ 1  , 1  , 0  , 0  , 1  , 1  , 0  , 1},
	{ 1  , 1  , 0  , 0  , 1  , 1  , 0  , 1},
	{ 1  , 1  , 0  , 0  , 1  , 1  , 0  , 1},
	{ 1  , 1  , 0  , 0  , 1  , 1  , 0  , 1},
	{ 1  , 1  , 0  , 1.5, 1  , 1  , 0  , 1},
	{ 0  , 1  , 1  , 0  , 1  , 1  , 0  , 1},
	{ 1  , 0  , 0  , 0  , 0  , 0  , 0  , 1}},

   {{ 1  , 0  , 0  , 1  , 1  , 1  , 1  , 1},
	{ 1  , 0  , 0  , 1  , 0  , 0  , 0  , 1},
	{ 1  , 0  , 1  , 1  , 0  , 0  , 0  , 1},
	{ 0  , 0  , 1  , 0  , 1  , 0  , 0  , 1},
	{ 0  ,1.21, 1  , 0  , 1  , 0  , 1.5, 1},
	{ 0  , 1  , 1  , 0  , 1  , 0  , 0  , 0},
	{ 0  , 0  , 0  , 0  , 1  , 0  , 0  , 0},
	{ 0  , 0  , 0  , 0  , 0  , 0  , 1.5, 1}},

   {{ 1  , 0  , 0  , 0  , 0  , 0  , 0  , 1},
	{ 0  , 0  , 0  , 0  , 0  , 0  , 0  , 1},
	{ 0  , 0  , 0  , 0  , 0  , 0  , 0  , 1},
	{ 0  , 0  , 0  , 0  , 0  , 0  , 0  , 1},
	{ 1  , 0  , 0  , 0  , 0  , 0  , 1  , 0},
	{ 0  , 0  , 0  , 0  , 0  , 0  , 0  , 1},
	{ 0  , 0  , 0  , 0  , 0  , 0  , 0  , 1},
	{ 0  , 1  , 1  , 1  , 1  , 1  , 0  , 1}},

   {{ 1  , 0  , 0  , 0  , 0  , 0  , 0  , 1},
	{ 0  , 0  , 0  , 0  , 1  , 1  , 1  , 1},
	{ 0  , 0  , 0  , 0  , 0  , 0  , 1  , 0},
	{ 1  , 0  , 0  , 0  , 0  , 0  , 0  , 0},
	{ 0  , 0  , 0  , 0  , 0  , 0  , 1  , 0},
	{ 1  , 0  , 1  , 1  , 0  , 0  , 1  , 1},
	{ 0  , 1.21,0  , 0  , 0  , 0  , 0  , 1},
	{ 0  , 1  , 0  , 1  , 1  , 1  , 0  , 1}},

   {{ 1  , 0  , 0  , 0  , 0  , 0  , 0  , 0},
	{ 1  , 0  , 0  , 0  , 0  , 0  , 0  , 0},
	{ 1  , 0  , 0  , 1  , 0  , 0  , 0  , 0},
	{ 0  , 0  , 0  , 1  , 0  , 0  , 0  , 0},
	{ 1  , 0  , 1  , 1  ,1.21, 0  , 0  , 0},
	{ 1  , 0  , 0  , 0  , 1  , 0  , 0  , 0},
	{ 0  , 0  , 0  , 0  , 0  , 0  , 1.5, 1},
	{ 1  , 1  , 1  , 0  , 0  , 0  , 0  , 1}},

   {{ 0  , 0  , 0  , 0  , 0  , 0  , 0  , 1},
	{ 1  , 0  , 0  , 1  , 0  , 0  , 0  , 0},
	{ 0  , 0  , 0  , 0  , 0  , 0  , 0  , 1},
	{ 0  , 0  , 0  , 0  , 1.21  , 0  , 0  , 0},
	{ 1  , 0  , 0  , 0  , 1  , 0  , 0  , 1},
	{ 0  , 0  , 1  , 1  , 0  , 0  , 0  , 1},
	{ 0  , 1  , 0  , 0  , 1  , 0  , 0  , 0},
	{ 0  , 0  , 1  , 0  , 0  , 0  ,1.5 , 1}},


};
static STAGEINFO g_stageInfo[STAGENUM]
{
	{STAGE_1,7,7,0,0,SOUND_LABEL_BGM_sample001},
	{STAGE_2,7,7,0,0,SOUND_LABEL_BGM_sample001},
	{STAGE_3,7,7,0,0,SOUND_LABEL_BGM_sample001},
	{STAGE_4,7,7,0,0,SOUND_LABEL_BGM_sample001},
	{STAGE_5,7,7,0,0,SOUND_LABEL_BGM_sample001},
	{STAGE_6,7,7,0,0,SOUND_LABEL_BGM_sample001},
};

#endif
static float g_rimtable[MAP_SIZE_HEIGHT][MAP_SIZE_WIDTH] = { 0.0f };




static char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/Blocks/Blank.png",				//�󂫂Ԃ�����@	�i�ʉߕs�j
	"data/TEXTURE/Blocks/Deep Stone 4.png",			//�f�t�H���g�u���b�N �i�ʉ߉j
	"data/TEXTURE/Blocks/Sandstone Stairs 3.png",	//�K�i���E�@�i�ʉߕs�@�K�i���聛�j
	"data/TEXTURE/Blocks/framePre.png",
	"data/TEXTURE/stageclear.png",
	"data/TEXTURE/Blocks/Sandstone Stairs 4.png",   //�K�i�㉺�@�i�ʉߕs�@�K�i���聛�j
	"data/TEXTURE/flag animation.png",				//�S�[��
	"data/TEXTURE/Blocks/redrim.png",
	"data/TEXTURE/Blocks/greenrim.png",
	"data/TEXTURE/Blocks/ArrowUp.png",
	"data/TEXTURE/Blocks/ArrowDown.png",
	"data/TEXTURE/Blocks/ArrowLeft.png",
	"data/TEXTURE/Blocks/ArrowRight.png",
	"data/TEXTURE/Blocks/Blue 4.png",				//�u���[�u���b�N�@�i�ʉ߉@�ړ��s�j
	"data/TEXTURE/Blocks/Magma 4.png",			//���b�h�u���b�N�@�i�ʉߕs�@�ړ��@�o�^�[�i�C�t���聛�j�@
	"data/TEXTURE/shadow000.jpg",
	"data/TEXTURE/UI ICONS_WHITE_pixel art_Sprite Sheet.png",
	"data/TEXTURE/Small-8-Direction-Characters_by_AxulArt.png",

};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitField(void)
{
	ID3D11Device* pDevice = GetDevice();

	//�e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	g_Load = TRUE;


	//g_Stage = 0;
	//if (GetLoadCheck())
	//	LoadData();
	g_select = FALSE;
	//g_check = TRUE;
	g_BlockInput = B_IDLE;
	g_InputDone = FALSE;

	g_move = 0;
	g_clear = FALSE;
	g_Menu_Flag = FALSE;
	g_Menu_Cursor = 0;


	LoadStage();
	g_Inputable = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitField(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	g_Load = FALSE;
	//g_Stage = ;
}
//=============================================================================
// �X�V����
//=============================================================================
void UpdateField(void)
{
	g_draw_frame_flag = FALSE;
	static int RestartCD = 0;
	if (g_Menu_Flag)
	{
		if (!*GetOptionFlag())
		{
			if (GetKeyboardTrigger(DIK_ESCAPE))
			{
				g_Menu_Flag = FALSE;
				PlaySound(SOUND_LABEL_SE_Deny);
				GetPlayer()[0].use = TRUE;
				g_Menu_Cursor = 0;
				GetInputStates()->T_ESC = TRUE;
			}
			if (GetKeyboardTrigger(DIK_DOWN))
			{
				PlaySound(SOUND_LABEL_SE_menu1);
				g_Menu_Cursor++;
				if (g_Menu_Cursor > 2)
					g_Menu_Cursor = 2;
				GetInputStates()->T_DOWN = TRUE;
			}
			else if (GetKeyboardTrigger(DIK_UP))
			{
				PlaySound(SOUND_LABEL_SE_menu1);
				g_Menu_Cursor--;
				g_Menu_Cursor %= MENU_OPS;
				if (g_Menu_Cursor < 0)
					g_Menu_Cursor = 0;
				GetInputStates()->T_UP = TRUE;
			}
			else if (GetKeyboardTrigger(DIK_RETURN))
			{
				//GetInputStates()->T_ENTER = TRUE;
				switch (g_Menu_Cursor)
				{
				case 0:
					PlaySound(SOUND_LABEL_SE_confirm);
					g_Stage_Clear = g_Stage;
					if (g_Stage_Clear > 0)
						*GetGReturn() = TRUE;
					g_Stage = 0;
					*GetReturn2() = TRUE;
					SetFade(FADE_OUT, MODE_TITLE);
					break;
				case 1:
					PlaySound(SOUND_LABEL_SE_confirm);
					*GetOptionFlag() = TRUE;
					break;
				case 2:
					PlaySound(SOUND_LABEL_SE_confirm);
					*GetExit() = TRUE;
					break;

				}
			}
		}


	}
	else
	{
		if ((GetBullet()->use) && (g_Inputable))
		{
			int x, y;
			x = (int)GetBullet()->pos.x;
			y = (int)GetBullet()->pos.y;
			if ((GetKeyboardTrigger(DIK_RETURN)) && (!g_select))
			{
				//GetInputStates()->T_ENTER = TRUE;
				if ((!g_select) && (PassCheck(STAGENUM, MAP_SIZE_WIDTH, MAP_SIZE_HEIGHT, (float*)g_Mapnow, 0, x, y)))
				{
					g_select = TRUE;
					memset(g_rimtable, 0, sizeof(g_rimtable));
					RimCheck(x, y);

					//g_check = TRUE;

				}


			}
			if ((GetKeyboardTrigger(DIK_ESCAPE)) && (g_select))
			{
				//GetInputStates()->T_ESC = TRUE;
				if (g_BlockInput)
				{
					g_BlockInput = B_IDLE;
					memset(g_rimtable, 0, sizeof(g_rimtable));
					RimCheck(x, y);
					PlaySound(SOUND_LABEL_SE_cancel);
				}
				else
				{
					g_First_Hover = 0;
					g_select = FALSE;
					PlaySound(SOUND_LABEL_SE_cancel);
					memset(g_rimtable, 0, sizeof(g_rimtable));
				}


			}

			if (g_select)
			{
				memset(g_rimtable, 0, sizeof(g_rimtable));
				RimCheck(x, y);
				BlockMoveInput();
			}
			if ((g_select) && (g_BlockInput))
			{
				if (GetKeyboardTrigger(DIK_RETURN))
				{
					//GetInputStates()->T_ENTER = TRUE;
					PlaySound(SOUND_LABEL_SE_move_confirm);
					g_InputDone = TRUE;
					WriteOutMap(g_move);
					WriteInMap(g_move);
					g_InputDone = FALSE;
					g_select = FALSE;
					g_BlockInput = B_IDLE;
					g_move = 0;

				}

			}

		}
		else
		{
			if (GetKeyboardTrigger(DIK_ESCAPE) && (!g_Menu_Flag))
			{
				GetInputStates()->T_ESC = TRUE;
				g_Menu_Flag = TRUE;
				PlaySound(SOUND_LABEL_SE_menu1);
				GetPlayer()[0].use = FALSE;
				g_Menu_Cursor = 0;

			}

		}
		if (GetKeyboardRepeat(DIK_Q) || (GetKeyboardPress(DIK_Q)))
		{
			GetInputStates()->T_ABI2 = TRUE;

			g_draw_frame_flag = TRUE;
		}
		else
		{
			g_draw_frame_flag = FALSE;
		}
		g_clear = ClearCheck();
		if (g_clear)
		{
			GetPlayer()->inputmode = INPUT_PAUSE;
			//g_Inputable = FALSE;
			if (GetKeyboardTrigger(DIK_RETURN))
			{
				//GetInputStates()->T_ENTER = TRUE;
				g_shift = TRUE;

			}
		}
		if (RestartCD)
			RestartCD--;
		if (GetKeyboardTrigger(DIK_R) && (!RestartCD))
		{
			//GetInputStates()->T_RESET = TRUE;
			GetPlayer()->inputmode = INPUT_GAME;
			GetPlayer()[0].use = TRUE;
			g_Reboot = TRUE;
			RestartCD = 100;
			StopSound(SOUND_LABEL_BGM_sample001);
			PlaySound(SOUND_LABEL_SE_restart);
			ClearStair();
		}

		if (g_Reboot)
		{
			g_shift = TRUE;
			g_Reboot = FALSE;
			g_Stage--;
			PlaySound(SOUND_LABEL_BGM_sample001);
		}

	}


}

void DrawField(void)
{
	UINT stride = sizeof(VERTEX_3D);


	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	int px, py;

	//�v���C���[�̈ʒu�������ăv���C���[��`�悷�鏇�Ԃ��m�肷��
	for (int x = 0; x < MAP_SIZE_HEIGHT; x++)
	{
		for (int y = 0; y < MAP_SIZE_WIDTH; y++)
		{
			if ((((int)GetPlayer()[0].sprite.pos.x == x) && ((int)GetPlayer()[0].sprite.pos.y == y))
				|| ((GetPlayer()[0].sprite.pos.x > x - 0.3f) && ((int)GetPlayer()[0].sprite.pos.y == y) && (g_Mapnow[x][y] == (int)g_Mapnow[x][y]))
				|| ((GetPlayer()[0].sprite.pos.y > y - 0.3f) && ((int)GetPlayer()[0].sprite.pos.x == x) && (g_Mapnow[x][y] == (int)g_Mapnow[x][y]))
				|| ((GetPlayer()[0].sprite.pos.y > y - 0.3f) && (GetPlayer()[0].sprite.pos.x > x - 0.3) && (g_Mapnow[x][y] == (int)g_Mapnow[x][y]))
				/*|| ((GetPlayer()[0].sprite.pos.y > y - 0.3f) && (GetPlayer()[0].sprite.pos.x > x + 0.7) && (g_Mapnow[x][y] == (int)g_Mapnow[x][y]))*/
				)

			{
				//(px,py)�u���b�N��`�悷���A�v���C���[��`��
				px = x;
				py = y;

			}
		}
	}
	int stair = GetStair();
	if (stair == 1)
	{
		px = GetStairBlock()->x;
		py = GetStairBlock()->y;
		if (PassCheck(STAGENUM, MAP_SIZE_WIDTH, MAP_SIZE_HEIGHT, (float*)g_Mapnow, 0, px + 1, py + 1) == STAIRUD)
		{
			px++;
			py += 2;
		}

	}
	if (stair == 2)
	{
		px = GetStairBlock()->x;
		py = GetStairBlock()->y;
		if (PassCheck(STAGENUM, MAP_SIZE_WIDTH, MAP_SIZE_HEIGHT, (float*)g_Mapnow, 0, px + 1, py + 1) == STAIRLR)
		{
			px += 2;
			py++;
		}

	}
	if (px > 7)
		px = 7;
	if (py > 7)
		py = 7;
	if (py < 0)
		py = 0;
	if (px < 0)
		px = 0;
	//����i0�C0�j����񂲂ƕ`�悷��
	for (int x = 0; x < MAP_SIZE_HEIGHT; x++)
	{
		for (int y = 0; y < MAP_SIZE_WIDTH; y++)
		{
			


			int index = (int)g_Mapnow[x][y];
			float adjust = g_Mapnow[x][y] - (int)(g_Mapnow[x][y]);
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[index]);

			SetSprite(g_VertexBuffer, DEFAULT_X - MAPCHIP_DISP * 0.5f * x + MAPCHIP_DISP * 0.5f * y, DEFAULT_Y + MAPCHIP_DISP * 0.25f * x + MAPCHIP_DISP * 0.25f * y, POLI_DISP, POLI_DISP, 0.0f, 0.0f, 1.0f, 1.0f);
			GetDeviceContext()->Draw(4, 0);

			if (g_draw_frame_flag)
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);

				SetSpriteColor(g_VertexBuffer, DEFAULT_X - MAPCHIP_DISP * 0.5f * x + MAPCHIP_DISP * 0.5f * y, DEFAULT_Y + MAPCHIP_DISP * 0.25f * x + MAPCHIP_DISP * 0.25f * y, POLI_DISP, POLI_DISP, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 0.3f));
				GetDeviceContext()->Draw(4, 0);
			}


			if (adjust)
			{
				float trans = 1.0f;
				if (g_draw_frame_flag)
					trans = 0.25f;
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[(int)(10 * adjust)]);
				SetSpriteColor(g_VertexBuffer, DEFAULT_X - MAPCHIP_DISP * 0.5f * x + MAPCHIP_DISP * 0.5f * y, DEFAULT_Y + MAPCHIP_DISP * 0.25f * x + MAPCHIP_DISP * 0.25f * y - MAPCHIP_DISP * 0.5f, POLI_DISP, POLI_DISP, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, trans));
				GetDeviceContext()->Draw(4, 0);
			}

			if (px == x && py == y)
			{
	

					DrawPlayer();



						//�@�O��֌W�𐳂����\�����邽�߁A�v���C���[���͂̊K�i�u���b�N�𕔕��I�ɍĕ`��
				if ((g_Mapnow[x - 1][y + 1] != (int)g_Mapnow[x - 1][y + 1]) && (x > 0) && (!GetStair()) && !g_draw_frame_flag)
				{
					
					for (int j = py + 1; j < MAP_SIZE_WIDTH; j++)
					{
						GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[(int)(10 * (g_Mapnow[px - 1][j] - (int)g_Mapnow[px - 1][j]))]);
						SetSpriteColor(g_VertexBuffer, DEFAULT_X - MAPCHIP_DISP * 0.5f * (px - 1) + MAPCHIP_DISP * 0.5f * (j), DEFAULT_Y + MAPCHIP_DISP * 0.25f * (px - 1) + MAPCHIP_DISP * 0.25f * (j)-MAPCHIP_DISP * 0.5f, POLI_DISP, POLI_DISP, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
						GetDeviceContext()->Draw(4, 0);
					}


				}

			}
			if (fx == x && fy == y)
			{
				DrawFlag(fx, fy);
				if (px == fx && py == fy)
				{
					if ((GetPlayer()[0].sprite.pos.x > (fx + 0.5f)) && (GetPlayer()[0].sprite.pos.y > (fy + 0.5f)))
						DrawPlayer();
				}
			}

		}
	}

	if (GetBullet()[0].use)
	{
		if (!g_select)
		{
			Drawrim((int)GetBullet()->pos.x, (int)GetBullet()->pos.y);
		}

		else
		{
			for (int x = 0; x < MAP_SIZE_HEIGHT; x++)
			{
				for (int y = 0; y < MAP_SIZE_WIDTH; y++)
				{
					if (g_rimtable[x][y])
					{
						if (!g_BlockInput)
						{
							DrawHover(x, y);
						}

					}
				}
			}
		}
	}


	if (g_select)
	{
		DrawArrowSelect();
	}
	if (g_BlockInput)
	{
		DrawBlockMovePreview();
	}

	if (g_clear)
		DrawClear();

	DrawUI();

	if (g_shift)
	{
		DrawShiftOut();

	}
	if (!g_Inputable)
	{

		DrawShiftIn();

	}


}

int* GetStage(void)
{
	return &g_Stage;
}

float* GetMC(void)
{
	return (float*)g_Mapnow;
}

//�@�}�b�v�`�b�v�̒l�����ƂɃv���C���[���ʉ߂��邩�ǂ��������߂�
float PassCheck(int depth, int width, int height, float* MappChipList, int z, int x, int y)
{
	if ((y < 0) || (x < 0) || (x > 7.7f) || (y > 7.7f))
	{
		return UNPASS;
	}
	//else if (!MappChipList[z * height * width + x * height + y])
	//{
	//	return 0.0f;
	//}
	else if (MappChipList[z * height * width + x * height + y] - (int)MappChipList[z * height * width + x * height + y] < 0.22f)
	{
		if (MappChipList[z * height * width + x * height + y] - (int)MappChipList[z * height * width + x * height + y] > 0.1f)
			return STAIRLR;
	}
	else if (MappChipList[z * height * width + x * height + y] - (int)MappChipList[z * height * width + x * height + y] == 0.5f)
	{
		return STAIRUD;
	}
	return (MappChipList[z * height * width + x * height + y]);

}


void DrawFlag(int x, int y)
{
	static int aniCount = 0;
	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);


	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[6]);

	float px = DEFAULT_X - x * MAPCHIP_WIDTH * MAP_SIZE_MOD * 0.5f + y * MAPCHIP_WIDTH * MAP_SIZE_MOD * 0.5f + 36;												// �S�[���\��X���W
	float py = DEFAULT_Y + x * MAPCHIP_WIDTH * MAP_SIZE_MOD * 0.25f + y * MAPCHIP_WIDTH * MAP_SIZE_MOD * 0.25f - 0.25f * MAPCHIP_WIDTH * MAP_SIZE_MOD + 6;		// �S�[���\��Y���W
	float pw = 100;		// �v���C���[�̕\����
	float ph = 100;		// �v���C���[�̕\������

	// �A�j���[�V�����p
	float tw = 0.2f;	// �e�N�X�`���̕�
	float th = 1.0f;	// �e�N�X�`���̍���
	float tx = 0.2f * (int)(aniCount / ANIM_WAIT);                      	// �e�N�X�`���̍���X���W
	float ty = 0.0f;	// �e�N�X�`���̍���Y���W

	SetSpriteColorRotationBottomUp(g_VertexBuffer, px, py, pw, ph,
		tx, ty, tw, th,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		0.0f);
	GetDeviceContext()->Draw(4, 0);

	if (!GetBullet()->use)
	{
		aniCount++;
		aniCount %= (FLAG_ANIM_PATTERN_NUM * ANIM_WAIT);
	}

}
void Drawrim(int x, int y)
{
	static int oldx = x;
	static int oldy = y;
	if (!((oldx == x) && (oldy == y)))
		PlaySound(SOUND_LABEL_SE_rim_scan);
	oldx = x;
	oldy = y;
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[7]);

	SetSprite(g_VertexBuffer, DEFAULT_X - MAPCHIP_DISP * 0.5f * x + MAPCHIP_DISP * 0.5f * y, DEFAULT_Y + MAPCHIP_DISP * 0.25f * x + MAPCHIP_DISP * 0.25f * y, POLI_DISP, POLI_DISP, 0.0f, 0.0f, 1.0f, 1.0f);
	GetDeviceContext()->Draw(4, 0);
}

void DrawHover(int x, int y)
{

	if (!g_First_Hover)
	{
		PlaySound(SOUND_LABEL_SE_move_select);
		g_First_Hover++;
	}

	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[8]);

	SetSpriteColor(g_VertexBuffer, DEFAULT_X - MAPCHIP_DISP * 0.5f * x + MAPCHIP_DISP * 0.5f * y, DEFAULT_Y + MAPCHIP_DISP * 0.25f * x + MAPCHIP_DISP * 0.25f * y, POLI_DISP, POLI_DISP, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	GetDeviceContext()->Draw(4, 0);

}


void RimCheck(int x, int y)
{
	float check = PassCheck(STAGENUM, MAP_SIZE_WIDTH, MAP_SIZE_HEIGHT, (float*)g_Mapnow, 0, x, y);
	PLAYER* player = GetPlayer();
	if (((int)player->sprite.pos.x == x) && ((int)player->sprite.pos.y == y))
	{
		//g_check = FALSE;
		g_select = FALSE;
		memset(g_rimtable, 0, sizeof(g_rimtable));
	}
	if ((!g_rimtable[x][y]) && (check) && (g_select))
	{
		g_rimtable[x][y] = g_Mapnow[x][y];
		if (g_select)
			RimCheck(x - 1, y);
		if (g_select)
			RimCheck(x + 1, y);
		if (g_select)
			RimCheck(x, y + 1);
		if (g_select)
			RimCheck(x, y - 1);

	}

}

void BlockMoveInput(void)
{

	if (GetKeyboardTrigger(DIK_UP))
	{
		g_BlockInput = B_UP;
	}
	else if (GetKeyboardTrigger(DIK_DOWN))
	{
		g_BlockInput = B_DOWN;
	}
	else if (GetKeyboardTrigger(DIK_LEFT))
	{
		g_BlockInput = B_LEFT;
	}
	else if (GetKeyboardTrigger(DIK_RIGHT))
	{
		g_BlockInput = B_RIGHT;
	}

	if (g_BlockInput)
	{
		if (GetKeyboardTrigger(DIK_ESCAPE))
		{
			g_BlockInput = B_IDLE;
		}
		//g_InputDone = TRUE;
		g_move = UpdateBlock();

	}
}

//�u���b�N�������Ƃ��\������A���[��`��
void DrawArrowSelect(void)
{
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	//�A���[�𓮂������߂̕ϐ�
	static int adjust = 0.0f;
	adjust++;
	adjust %= 20;
	BULLET* bullet = GetBullet();
	float px = (int)bullet[0].pos.x; // �I�����Ă�u���b�N�ʒuX
	float py = (int)bullet[0].pos.y; // �I�����Ă�u���b�N�ʒuY
	float xu, yu, xd, yd, xl, yl, xr, yr;
	//��A���[
	xu = DEFAULT_X - MAPCHIP_DISP * 0.5f * px + MAPCHIP_DISP * 0.5f * (py - 1);	//�u���b�N���W���烂�j�^�[���W�֕ϊ�
	yu = DEFAULT_Y + MAPCHIP_DISP * 0.25f * px + MAPCHIP_DISP * 0.25f * (py - 1);//�u���b�N���W���烂�j�^�[���W�֕ϊ�
	//���A���[
	xd = DEFAULT_X - MAPCHIP_DISP * 0.5f * px + MAPCHIP_DISP * 0.5f * (py + 1);
	yd = DEFAULT_Y + MAPCHIP_DISP * 0.25f * px + MAPCHIP_DISP * 0.25f * (py + 1);
	//���A���[
	xl = DEFAULT_X - MAPCHIP_DISP * 0.5f * (px + 1) + MAPCHIP_DISP * 0.5f * (py);
	yl = DEFAULT_Y + MAPCHIP_DISP * 0.25f * (px + 1) + MAPCHIP_DISP * 0.25f * (py);
	//�E�A���[
	xr = DEFAULT_X - MAPCHIP_DISP * 0.5f * (px - 1) + MAPCHIP_DISP * 0.5f * (py);
	yr = DEFAULT_Y + MAPCHIP_DISP * 0.25f * (px - 1) + MAPCHIP_DISP * 0.25f * (py);

	float pw = POLI_DISP;		// Arrow�\����
	float ph = POLI_DISP;		// Arrow�\������

	float tw = 1.0f;	// �e�N�X�`���̕�
	float th = 1.0f;	// �e�N�X�`���̍���
	float tx = 0.0f;	// �e�N�X�`���̍���X���W
	float ty = 0.0f;	// �e�N�X�`���̍���Y���W
	switch (g_BlockInput)
	{
	case B_UP:


		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[10]);
		SetSpriteColorRotation(g_VertexBuffer, xd, yd, pw, ph, tx, ty, tw, th,
			XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			0.0f);
		GetDeviceContext()->Draw(4, 0);

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[11]);
		SetSpriteColorRotation(g_VertexBuffer, xl, yl, pw, ph, tx, ty, tw, th,
			XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			0.0f);
		GetDeviceContext()->Draw(4, 0);

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[12]);
		SetSpriteColorRotation(g_VertexBuffer, xr, yr, pw, ph, tx, ty, tw, th,
			XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			0.0f);
		GetDeviceContext()->Draw(4, 0);

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[9]);

		SetSpriteColorRotation(g_VertexBuffer, DEFAULT_X - MAPCHIP_DISP * 0.5f * px + MAPCHIP_DISP * 0.5f * (py - 1 - (float)adjust * 0.01f), DEFAULT_Y + MAPCHIP_DISP * 0.25f * px + MAPCHIP_DISP * 0.25f * (py - 1 - (float)adjust * 0.01f), pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
			0.0f);
		GetDeviceContext()->Draw(4, 0);

		break;
	case B_DOWN:
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[10]);
		SetSpriteColorRotation(g_VertexBuffer, DEFAULT_X - MAPCHIP_DISP * 0.5f * px + MAPCHIP_DISP * 0.5f * (py + 1 + (float)adjust * 0.01f), DEFAULT_Y + MAPCHIP_DISP * 0.25f * px + MAPCHIP_DISP * 0.25f * (py + 1 + (float)adjust * 0.01f), pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
			0.0f);
		GetDeviceContext()->Draw(4, 0);

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[11]);
		SetSpriteColorRotation(g_VertexBuffer, xl, yl, pw, ph, tx, ty, tw, th,
			XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			0.0f);
		GetDeviceContext()->Draw(4, 0);

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[12]);
		SetSpriteColorRotation(g_VertexBuffer, xr, yr, pw, ph, tx, ty, tw, th,
			XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			0.0f);
		GetDeviceContext()->Draw(4, 0);

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[9]);

		SetSpriteColorRotation(g_VertexBuffer, xu, yu, pw, ph, tx, ty, tw, th,
			XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			0.0f);
		GetDeviceContext()->Draw(4, 0);
		break;
	case B_LEFT:
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[10]);
		SetSpriteColorRotation(g_VertexBuffer, xd, yd, pw, ph, tx, ty, tw, th,
			XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			0.0f);
		GetDeviceContext()->Draw(4, 0);

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[11]);
		SetSpriteColorRotation(g_VertexBuffer, DEFAULT_X - MAPCHIP_DISP * 0.5f * (px + 1 + (float)adjust * 0.01f) + MAPCHIP_DISP * 0.5f * (py), DEFAULT_Y + MAPCHIP_DISP * 0.25f * (px + 1 + (float)adjust * 0.01f) + MAPCHIP_DISP * 0.25f * (py), pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
			0.0f);
		GetDeviceContext()->Draw(4, 0);

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[12]);
		SetSpriteColorRotation(g_VertexBuffer, xr, yr, pw, ph, tx, ty, tw, th,
			XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			0.0f);
		GetDeviceContext()->Draw(4, 0);

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[9]);

		SetSpriteColorRotation(g_VertexBuffer, xu, yu, pw, ph, tx, ty, tw, th,
			XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			0.0f);
		GetDeviceContext()->Draw(4, 0);
		break;
	case B_RIGHT:
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[10]);
		SetSpriteColorRotation(g_VertexBuffer, xd, yd, pw, ph, tx, ty, tw, th,
			XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			0.0f);
		GetDeviceContext()->Draw(4, 0);

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[11]);
		SetSpriteColorRotation(g_VertexBuffer, xl, yl, pw, ph, tx, ty, tw, th,
			XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			0.0f);
		GetDeviceContext()->Draw(4, 0);

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[12]);
		SetSpriteColorRotation(g_VertexBuffer, DEFAULT_X - MAPCHIP_DISP * 0.5f * (px - 1 - (float)adjust * 0.01f) + MAPCHIP_DISP * 0.5f * (py), DEFAULT_Y + MAPCHIP_DISP * 0.25f * (px - 1 - (float)adjust * 0.01f) + MAPCHIP_DISP * 0.25f * (py), pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
			0.0f);
		GetDeviceContext()->Draw(4, 0);

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[9]);

		SetSpriteColorRotation(g_VertexBuffer, xu, yu, pw, ph, tx, ty, tw, th,
			XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			0.0f);
		GetDeviceContext()->Draw(4, 0);
		break;
	case B_IDLE:
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[10]);
		SetSpriteColorRotation(g_VertexBuffer, xd, yd, pw, ph, tx, ty, tw, th,
			XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			0.0f);
		GetDeviceContext()->Draw(4, 0);

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[11]);
		SetSpriteColorRotation(g_VertexBuffer, xl, yl, pw, ph, tx, ty, tw, th,
			XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			0.0f);
		GetDeviceContext()->Draw(4, 0);

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[12]);
		SetSpriteColorRotation(g_VertexBuffer, xr, yr, pw, ph, tx, ty, tw, th,
			XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			0.0f);
		GetDeviceContext()->Draw(4, 0);

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[9]);

		SetSpriteColorRotation(g_VertexBuffer, xu, yu, pw, ph, tx, ty, tw, th,
			XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			0.0f);
		GetDeviceContext()->Draw(4, 0);
		break;
	}
}

BOOL GetSelect(void)
{
	return g_select;
}

int UpdateBlock(void)
{

	//	�ړ���[�̃����b�N��x,y���L�^

	BOOL passable = TRUE;
	int moveCount = 0;
	float temp_map[MAP_SIZE_HEIGHT][MAP_SIZE_WIDTH] = { 0 };

	for (int x = 0; x < MAP_SIZE_HEIGHT; x++)
	{
		for (int y = 0; y < MAP_SIZE_WIDTH; y++)
		{
			if (!g_rimtable[x][y])
				temp_map[x][y] = g_Mapnow[x][y];
		}
	}
	switch (g_BlockInput)
	{
	case B_UP:

		while (passable)
		{	//�P�R�}�����܂����H�@�iyes:passable = FALSE; no:passable = TRUE�j
			for (int x = 0; x < MAP_SIZE_HEIGHT; x++)
			{
				for (int y = 0; y < MAP_SIZE_WIDTH; y++)
				{
					if (g_rimtable[x][y] != 0.0f)
					{
						if (y == 0)
						{
							passable = FALSE;
						}
						if ((temp_map[x][y - 1] != 0.0f) /*&& (g_rimtable[x][y - 1] == 0.0f)*/)
						{
							passable = FALSE;
						}
					}
				}
			}
			//yes�Ȃ�
			if (passable)
			{
				for (int y = 1; y < MAP_SIZE_WIDTH; y++)
				{
					for (int x = 0; x < MAP_SIZE_HEIGHT; x++)
					{
						if (g_rimtable[x][y] > 0.0f)
						{
							g_rimtable[x][y - 1] = g_rimtable[x][y];
							g_rimtable[x][y] = 0.0f;
						}

					}

				}
				moveCount++;
			}

		}
		break;
	case B_DOWN:

		while (passable)
		{	//�P�R�}�����܂����H�@�iyes:passable = FALSE; no:passable = TRUE�j
			for (int x = 0; x < MAP_SIZE_HEIGHT; x++)
			{
				for (int y = 0; y < MAP_SIZE_WIDTH; y++)
				{
					if (g_rimtable[x][y] != 0.0f)
					{
						if (y == 7)
						{
							passable = FALSE;
						}
						if ((temp_map[x][y + 1] != 0.0f) /*&& (g_rimtable[x][y + 1] == 0.0f)*/)
						{
							passable = FALSE;
						}
					}
				}
			}
			//yes�Ȃ�
			if (passable)
			{
				for (int y = 6; y > -1; y--)
				{
					for (int x = 0; x < MAP_SIZE_HEIGHT; x++)
					{
						if (g_rimtable[x][y] > 0.0f)
						{
							g_rimtable[x][y + 1] = g_rimtable[x][y];
							g_rimtable[x][y] = 0.0f;
						}

					}

				}
				moveCount++;
			}

		}
		break;
	case B_LEFT:
		while (passable)
		{	//�P�R�}�����܂����H�@�iyes:passable = FALSE; no:passable = TRUE�j
			for (int x = 0; x < MAP_SIZE_HEIGHT; x++)
			{
				for (int y = 0; y < MAP_SIZE_WIDTH; y++)
				{
					if (g_rimtable[x][y] != 0.0f)
					{
						if (x == 7)
						{
							passable = FALSE;
						}
						if ((temp_map[x + 1][y] != 0)/* && (g_rimtable[x + 1][y] == 0.0f)*/)
						{
							passable = FALSE;
						}
					}
				}
			}
			//yes�Ȃ�
			if (passable)
			{
				for (int x = 6; x > -1; x--)
				{
					for (int y = 0; y < MAP_SIZE_WIDTH; y++)
					{
						if (g_rimtable[x][y] > 0.0f)
						{
							g_rimtable[x + 1][y] = g_rimtable[x][y];
							g_rimtable[x][y] = 0.0f;
						}

					}


				}
				moveCount++;
			}

		}
		break;
	case B_RIGHT:
		while (passable)
		{	//�P�R�}�����܂����H�@�iyes:passable = FALSE; no:passable = TRUE�j
			for (int x = 0; x < MAP_SIZE_HEIGHT; x++)
			{
				for (int y = 0; y < MAP_SIZE_WIDTH; y++)
				{
					if (g_rimtable[x][y] != 0.0f)
					{
						if (x == 0)
						{
							passable = FALSE;
						}
						if ((temp_map[x - 1][y] != 0) /*&& (g_rimtable[x - 1][y] == 0.0f)*/)
						{
							passable = FALSE;
						}
					}
				}
			}
			//yes�Ȃ�
			if (passable)
			{
				for (int x = 1; x < MAP_SIZE_HEIGHT; x++)
				{
					for (int y = 0; y < MAP_SIZE_WIDTH; y++)
					{
						if (g_rimtable[x][y] > 0.0f)
						{
							g_rimtable[x - 1][y] = g_rimtable[x][y];
							g_rimtable[x][y] = 0.0f;
						}

					}

				}
				moveCount++;
			}

		}
		break;
	}

	return moveCount;


}

void LoadStage(void)
{

	g_Inputable = FALSE;
	g_select = FALSE;
	fx = g_stageInfo[g_Stage].gx;
	fy = g_stageInfo[g_Stage].gy;
	g_BlockInput = B_IDLE;
	g_InputDone = FALSE;
	g_move = 0;
	g_clear = FALSE;
	g_shift = FALSE;
	GetBullet()->use = FALSE;
	GetPlayer()->sprite.pos.x = (float)g_stageInfo[g_Stage].px + 0.5f;
	GetPlayer()->sprite.pos.y = (float)g_stageInfo[g_Stage].py + 0.5f;
	GetPlayer()->inputmode = INPUT_GAME;

	for (int x = 0; x < MAP_SIZE_HEIGHT; x++)
	{
		for (int y = 0; y < MAP_SIZE_WIDTH; y++)
		{
			g_Mapnow[x][y] = g_MapChipList[g_Stage][x][y];
		}
	}
}


void DrawBlockMovePreview(void)
{
	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);


	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	for (int x = 0; x < MAP_SIZE_HEIGHT; x++)
	{
		for (int y = 0; y < MAP_SIZE_WIDTH; y++)
		{
			if (g_rimtable[x][y])
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);
				SetSpriteColor(g_VertexBuffer, DEFAULT_X - MAPCHIP_DISP * 0.5f * x + MAPCHIP_DISP * 0.5f * y, DEFAULT_Y + MAPCHIP_DISP * 0.25f * x + MAPCHIP_DISP * 0.25f * y, POLI_DISP, POLI_DISP, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(0.1f, 1.0f, 0.1f, 0.4f));
				GetDeviceContext()->Draw(4, 0);
			}


		}
	}

}


void WriteOutMap(int stepnum)
{
	if (stepnum)
	{
		switch (g_BlockInput)
		{
		case B_UP:
			for (int x = 0; x < MAP_SIZE_HEIGHT; x++)
			{
				for (int y = 0; y < MAP_SIZE_WIDTH; y++)
				{
					if (g_rimtable[x][y] > 0.0f)
					{
						g_Mapnow[x][y + stepnum] = 0.0f;
					}
				}
			}
			break;
		case B_DOWN:
			for (int x = 0; x < MAP_SIZE_HEIGHT; x++)
			{
				for (int y = 0; y < MAP_SIZE_WIDTH; y++)
				{
					if (g_rimtable[x][y] > 0.0f)
					{
						g_Mapnow[x][y - stepnum] = 0.0f;
					}
				}
			}
			break;

		case B_LEFT:
			for (int x = 0; x < MAP_SIZE_HEIGHT; x++)
			{
				for (int y = 0; y < MAP_SIZE_WIDTH; y++)
				{
					if (g_rimtable[x][y] > 0.0f)
					{
						g_Mapnow[x - stepnum][y] = 0.0f;
					}
				}
			}
			break;

		case B_RIGHT:
			for (int x = 0; x < MAP_SIZE_HEIGHT; x++)
			{
				for (int y = 0; y < MAP_SIZE_WIDTH; y++)
				{
					if (g_rimtable[x][y] > 0.0f)
					{
						g_Mapnow[x + stepnum][y] = 0.0f;
					}
				}
			}
			break;
		}

	}
}

void WriteInMap(int stepnum)
{
	if (stepnum)
	{
		for (int x = 0; x < MAP_SIZE_HEIGHT; x++)
		{
			for (int y = 0; y < MAP_SIZE_WIDTH; y++)
			{
				if (g_rimtable[x][y] > 0.0f)
				{
					g_Mapnow[x][y] = g_rimtable[x][y];
				}
			}
		}

	}
}

STAGEINFO* GetStageInfo(void)
{
	return &g_stageInfo[g_Stage];
}

BOOL ClearCheck(void)
{
	PLAYER* player = GetPlayer();
	XMFLOAT3 FlagPos = { (float)g_stageInfo[g_Stage].gx + 0.4f,(float)g_stageInfo[g_Stage].gy + 0.4f,0.0f };
	return CollisionBC(FlagPos, player->sprite.pos, 0.29f, 0.0001f);
}

void DrawClear(void)
{
	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);


	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[4]);

	SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X - 2, SCREEN_CENTER_Y, GOAL_SIZE, GOAL_SIZE, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	GetDeviceContext()->Draw(4, 0);
	SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X + 2, SCREEN_CENTER_Y, GOAL_SIZE, GOAL_SIZE, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	GetDeviceContext()->Draw(4, 0);
	SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X, SCREEN_CENTER_Y - 2, GOAL_SIZE, GOAL_SIZE, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	GetDeviceContext()->Draw(4, 0);
	SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X, SCREEN_CENTER_Y + 2, GOAL_SIZE, GOAL_SIZE, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	GetDeviceContext()->Draw(4, 0);
	SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X - 2, SCREEN_CENTER_Y + 2, GOAL_SIZE, GOAL_SIZE, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	GetDeviceContext()->Draw(4, 0);
	SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X - 2, SCREEN_CENTER_Y - 2, GOAL_SIZE, GOAL_SIZE, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	GetDeviceContext()->Draw(4, 0);
	SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X + 2, SCREEN_CENTER_Y + 2, GOAL_SIZE, GOAL_SIZE, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	GetDeviceContext()->Draw(4, 0);
	SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X + 2, SCREEN_CENTER_Y - 2, GOAL_SIZE, GOAL_SIZE, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	GetDeviceContext()->Draw(4, 0);
	SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X, SCREEN_CENTER_Y, GOAL_SIZE, GOAL_SIZE, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	GetDeviceContext()->Draw(4, 0);

	DIRECTWRITE* DW = GetDirectWrite();
	WCHAR wcText1[] = L"�@PRESS ENTER TO CONTINUE";
	DW[1].DWriteFactory->CreateTextFormat(L"���C���I", nullptr, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 30, L"", &DW[1].TextFormat);
	DW[1].RT->BeginDraw();
	DW[1].RT->DrawText(wcText1, ARRAYSIZE(wcText1) - 1, DW[1].TextFormat, D2D1::RectF(SCREEN_CENTER_X - 280, 720 - 50, SCREEN_CENTER_X + 800, 720 - 25), DW[1].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
	DW[1].RT->EndDraw();

}

void DrawShiftOut(void)
{
	static float Count = 0.0f;
	static float step = 50.0f;

	if (Count < 2000.0f)
	{
		Count += step;
	}

	SetBlendState(BLEND_MODE_SUBTRACT);
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[15]);
	PLAYER* player = GetPlayer();
	//�Ə��J�[�\���̈ʒu��e�N�X�`���[���W�𔽉f
	float px = DEFAULT_X - player[0].sprite.pos.x * MAPCHIP_WIDTH * MAP_SIZE_MOD * 0.5f + player[0].sprite.pos.y * MAPCHIP_WIDTH * MAP_SIZE_MOD * 0.5f;
	float py = DEFAULT_Y + player[0].sprite.pos.x * MAPCHIP_WIDTH * MAP_SIZE_MOD * 0.25f + player[0].sprite.pos.y * MAPCHIP_WIDTH * MAP_SIZE_MOD * 0.25f - 0.25f * MAPCHIP_WIDTH * MAP_SIZE_MOD;
	float pw = Count;		// �v���C���[�̕\����
	float ph = Count;		// �v���C���[�̕\������

	// �A�j���[�V�����p
	float tw = 1.0f;	// �e�N�X�`���̕�
	float th = 1.0f;	// �e�N�X�`���̍���
	float tx = 0.0f;                      	// �e�N�X�`���̍���X���W
	float ty = 0.0f;	// �e�N�X�`���̍���Y���W


	for (int i = 0; i < 10; i++)
	{
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph,
			tx, ty, tw, th,
			XMFLOAT4(0.8f, 0.8f, 0.8f, 0.5f));
		GetDeviceContext()->Draw(4, 0);
	}




	SetBlendState(BLEND_MODE_ALPHABLEND);
	if (Count == 2000.0f)
	{
		g_Stage++;
		g_Stage_Clear = g_Stage;

		if (g_Stage == STAGENUM)
		{
			g_Stage = 0;
			g_Stage_Clear = STAGENUM - 1;
			g_shift = FALSE;
			*GetGReturn() = TRUE;
			SetFade(FADE_OUT, MODE_RESULT);

		}
		else
		{
			Count = 0.0f;
			step = 50.0f;
			LoadStage();

		}


	}

}

void DrawShiftIn(void)
{
	static float Count = 2000.0f;
	static float step = -50.0f;

	if (Count > 0.0f)
	{
		Count += step;
	}


	SetBlendState(BLEND_MODE_SUBTRACT);
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[15]);
	PLAYER* player = GetPlayer();
	//�Ə��J�[�\���̈ʒu��e�N�X�`���[���W�𔽉f
	float px = DEFAULT_X - player[0].sprite.pos.x * MAPCHIP_WIDTH * MAP_SIZE_MOD * 0.5f + player[0].sprite.pos.y * MAPCHIP_WIDTH * MAP_SIZE_MOD * 0.5f;
	float py = DEFAULT_Y + player[0].sprite.pos.x * MAPCHIP_WIDTH * MAP_SIZE_MOD * 0.25f + player[0].sprite.pos.y * MAPCHIP_WIDTH * MAP_SIZE_MOD * 0.25f - 0.25f * MAPCHIP_WIDTH * MAP_SIZE_MOD;
	float pw = Count;		// �v���C���[�̕\����
	float ph = Count;		// �v���C���[�̕\������

	// �A�j���[�V�����p
	float tw = 1.0f;	// �e�N�X�`���̕�
	float th = 1.0f;	// �e�N�X�`���̍���
	float tx = 0.0f;                      	// �e�N�X�`���̍���X���W
	float ty = 0.0f;	// �e�N�X�`���̍���Y���W


	for (int i = 0; i < 10; i++)
	{
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph,
			tx, ty, tw, th,
			XMFLOAT4(0.8f, 0.8f, 0.8f, 0.5f));
		GetDeviceContext()->Draw(4, 0);
	}

	if (Count <= 0.0f)
	{
		g_Inputable = TRUE;
		Count = 2000.0f;
		step = -50.0f;
	}


	SetBlendState(BLEND_MODE_ALPHABLEND);

}

int* GetStageClear(void)
{
	return &g_Stage_Clear;
}

void DrawUI(void)
{

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[16]);
	if (!g_Menu_Flag)
	{
		SetSpriteLTColor(g_VertexBuffer, 10, 10, UI_BUTTON_SIZE * 0.5f, UI_BUTTON_SIZE * 0.5f,
			0.1f, 0.0f, 0.1f, 0.1111f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);
	}
	else if (g_Menu_Flag)
	{
		SetSpriteLTColor(g_VertexBuffer, 10, 10, UI_BUTTON_SIZE * 0.5f, UI_BUTTON_SIZE * 0.5f,
			0.1f, 0.0f, 0.1f, 0.1111f,
			XMFLOAT4(1.0f, 0.8f, 0.2f, 1.0f));
		GetDeviceContext()->Draw(4, 0);

		SetSpriteLTColor(g_VertexBuffer, 70, 75 + 65, UI_BUTTON_SIZE * 0.5f, UI_BUTTON_SIZE * 0.5f,
			0.3f, 0.1111f * 7, 0.1f, 0.1111f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);

		SetSpriteLTColor(g_VertexBuffer, 70, 10, UI_BUTTON_SIZE * 0.5f, UI_BUTTON_SIZE * 0.5f,
			0.6f, 0.1111f * 7, 0.1f, 0.1111f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);

		SetSpriteLTColor(g_VertexBuffer, 70, 75, UI_BUTTON_SIZE * 0.5f, UI_BUTTON_SIZE * 0.5f,
			0.5f, 0.1111f * 7, 0.1f, 0.1111f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);

		switch (g_Menu_Cursor)
		{
			//���j���[��
		case 0:

			SetSpriteLTColor(g_VertexBuffer, 70, 10, UI_BUTTON_SIZE * 0.5f, UI_BUTTON_SIZE * 0.5f,
				0.6f, 0.1111f * 7, 0.1f, 0.1111f,
				XMFLOAT4(0.2f, 1.0f, 0.2f, 1.0f));
			GetDeviceContext()->Draw(4, 0);

			break;
			//�ޏo
		case 2:

			SetSpriteLTColor(g_VertexBuffer, 70, 75 + 65, UI_BUTTON_SIZE * 0.5f, UI_BUTTON_SIZE * 0.5f,
				0.3f, 0.1111f * 7, 0.1f, 0.1111f,
				XMFLOAT4(0.2f, 1.0f, 0.2f, 1.0f));
			GetDeviceContext()->Draw(4, 0);
			break;
			//�I�v�V����
		case 1:

			SetSpriteLTColor(g_VertexBuffer, 70, 75, UI_BUTTON_SIZE * 0.5f, UI_BUTTON_SIZE * 0.5f,
				0.5f, 0.1111f * 7, 0.1f, 0.1111f,
				XMFLOAT4(0.2f, 1.0f, 0.2f, 1.0f));
			GetDeviceContext()->Draw(4, 0);

			break;
		}

	}



}

BOOL GetMenuFlag(void)
{
	return g_Menu_Flag;
}

BOOL GetBlockInput(void)
{
	return g_BlockInput;
}

int GetMenuCursor(void)
{
	return g_Menu_Cursor;
}

void DrawPlayer(void)
{
	//�e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[17]);

	//�Ə��J�[�\���̈ʒu��e�N�X�`���[���W�𔽉f
	float px = DEFAULT_X - GetPlayer()[0].sprite.pos.x * MAPCHIP_WIDTH * MAP_SIZE_MOD * 0.5f + GetPlayer()[0].sprite.pos.y * MAPCHIP_WIDTH * MAP_SIZE_MOD * 0.5f;		// �v���C���[�̕\���ʒuX
	float py = DEFAULT_Y + GetPlayer()[0].sprite.pos.x * MAPCHIP_WIDTH * MAP_SIZE_MOD * 0.25f + GetPlayer()[0].sprite.pos.y * MAPCHIP_WIDTH * MAP_SIZE_MOD * 0.25f - 0.25f * MAPCHIP_WIDTH * MAP_SIZE_MOD;		// �v���C���[�̕\���ʒuY
	float pw = GetPlayer()[0].sprite.w;		// �v���C���[�̕\����
	float ph = GetPlayer()[0].sprite.h;		// �v���C���[�̕\������

	// �A�j���[�V�����p
	float tw = 1.0f / (TEXTURE_PATTERN_DIVIDE_X * 8);	// �e�N�X�`���̕�
	float th = 0.09090909f;	// �e�N�X�`���̍���
	float tx =0.0f;                      	// �e�N�X�`���̍���X���W
	float ty = 0.07575758f + (float)(GetPlayer()[0].sprite.patternAnim % TEXTURE_PATTERN_DIVIDE_Y) * th;	// �e�N�X�`���̍���Y���W

	switch (GetPlayer()[0].sprite.facing)
	{
	case F_UP:
		tx = 0.0f;
		break;
	case F_RU:
		tx = 0.125f;
		break;
	case F_RIGHT:
		tx = 0.25f;
		break;
	case F_RD:
		tx = 0.375f;
		break;
	case F_DOWN:
		tx = 0.5f;
		break;
	case F_LD:
		tx = 0.625f;
		break;
	case F_LEFT:
		tx = 0.75f;
		break;
	case F_LU:
		tx = 0.875f;
		break;
	}

	// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	SetSpriteColorRotationBottomUp(g_VertexBuffer, px, py, pw, ph,
		tx, ty, tw, th,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		GetPlayer()[0].sprite.rot.z);


	GetDeviceContext()->Draw(4, 0);
}