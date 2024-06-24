
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "enemy.h"
#include "collision.h"
#include "score.h"
#include "file.h"
#include "sprite.h"
#include "field.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(160/3)	// �L�����T�C�Y
#define TEXTURE_HEIGHT				(220/3)	// 
#define TEXTURE_MAX					(2)		// �e�N�X�`���̐�

#define TEXTURE_PATTERN_DIVIDE_X	(1)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(3)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define ANIM_WAIT					(8)		// �A�j���[�V�����̐؂�ւ��Wait�l

// �W�����v����
#define	PLAYER_JUMP_CNT_MAX			(30)		// 30�t���[���Œ��n����
#define	PLAYER_JUMP_Y_MAX			(300.0f)	// �W�����v�̍���

#define CURSOR_SPEED                (5.0f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
//void DrawPlayerOffset(int no);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/Small-8-Direction-Characters_by_AxulArt.png",
	"data/TEXTURE/white_fuchi.png",
};


static BOOL		  g_Load = FALSE;			// ���������s�������̃t���O
static PLAYER	  g_Player[PLAYER_MAX];   	// �v���C���[�\����
static int        g_stair = 0;
static StairStats g_stairBlock;				// ����Ă�K�i�u���b�N�̍��W

//static SPRITE	g_Cursor;				    // �Ə��J�[�\���\����
//static SPRITE	g_Aimline;			     	// �Ə����\����

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
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

	// �v���C���[�\���̂̏�����
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		g_Player[i].sprite.use = TRUE;
		g_Player[i].sprite.pos = XMFLOAT3(7.5f, 7.5f, 0.0f);	// ���S�_����\��
		g_Player[i].sprite.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Player[i].sprite.w = TEXTURE_WIDTH;
		g_Player[i].sprite.h = TEXTURE_HEIGHT;
		g_Player[i].sprite.texNo = 0;
		g_Player[i].sprite.countAnim = 0;
		g_Player[i].sprite.patternAnim = 1;
		g_Player[i].sprite.facing = F_UP;
		g_Player[i].speed = 0.04f;
		g_Player[i].moving = FALSE;
		g_Player[i].inputmode = INPUT_GAME;
		g_Player[i].use = TRUE;
#ifdef _DEBUG
		g_Player[i].inputmode = INPUT_GAME;
#endif 


		//g_Player[i].speed = XMFLOAT3(0.0f, 0.0f, 0.0f);		// ���x
		//g_Player[i].acc = XMFLOAT3(0.0f, 0.0f, 0.0f);		// �����x


	}



	g_Load = TRUE;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
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
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{

	for (int i = 0; i < PLAYER_MAX; i++)
	{
		switch (g_Player[i].inputmode)
		{
		case INPUT_TITLE:
			break;

		case INPUT_TUIT:
			break;
		case INPUT_GAME:
			UpdatePre();
			break;
		case INPUT_PAUSE:

			break;

		}



#ifdef _DEBUG	// �f�o�b�O����\������


#endif

	}
}
//=============================================================================
// �w�ߗA�����̍X�V����
//=============================================================================
void UpdatePre()
{
	XMFLOAT3 old_pos = g_Player[0].sprite.pos;
	int oldFacing = g_Player[0].sprite.facing;

	g_Player[0].sprite.countAnim++;
	// �A�j���[�V����  
	if (g_Player[0].moving == TRUE)
	{

		if (g_Player[0].sprite.countAnim > ANIM_WAIT)
		{
			g_Player[0].sprite.countAnim = 0;
			// �p�^�[���̐؂�ւ�
			g_Player[0].sprite.patternAnim = (g_Player[0].sprite.patternAnim + 1) % TEXTURE_PATTERN_DIVIDE_Y;
		}
	}
	else
	{
		if (g_Player[0].sprite.countAnim > ANIM_WAIT)
		{
			g_Player[0].sprite.countAnim = 0;
			// �p�^�[���̐؂�ւ�
			g_Player[0].sprite.patternAnim = 1;
		}
	}

	g_Player[0].moving = FALSE;
	if (g_Player[0].use)
	{
		g_Player[0].sprite.facing = F_IDLE;

		if (!g_stair)
		{

			if (GetKeyboardPress(DIK_RIGHT))
			{
				g_Player[0].sprite.pos.y += g_Player[0].speed;
				g_Player[0].sprite.pos.x -= g_Player[0].speed;
				g_Player[0].sprite.facing ^= F_RIGHT;
				g_Player[0].moving = TRUE;
			}
			else if (GetKeyboardPress(DIK_LEFT))
			{
				g_Player[0].sprite.pos.y -= g_Player[0].speed;
				g_Player[0].sprite.pos.x += g_Player[0].speed;
				g_Player[0].sprite.facing ^= F_LEFT;
				g_Player[0].moving = TRUE;
			}

			if (GetKeyboardPress(DIK_DOWN))
			{

				g_Player[0].sprite.pos.x += g_Player[0].speed;
				g_Player[0].sprite.pos.y += g_Player[0].speed;
				g_Player[0].sprite.facing ^= F_DOWN;
				g_Player[0].moving = TRUE;
			}
			else if (GetKeyboardPress(DIK_UP))
			{

				g_Player[0].sprite.pos.x -= g_Player[0].speed;
				g_Player[0].sprite.pos.y -= g_Player[0].speed;
				g_Player[0].sprite.facing ^= F_UP;
				g_Player[0].moving = TRUE;
			}

			// �Q�[���p�b�h�łňړ�����
			if (IsButtonPressed(0, BUTTON_DOWN))
			{
				g_Player[0].sprite.pos.x += g_Player[0].speed;
				g_Player[0].sprite.pos.y += g_Player[0].speed;
				g_Player[0].sprite.facing ^= F_DOWN;
				g_Player[0].moving = TRUE;
			}
			else if (IsButtonPressed(0, BUTTON_UP))
			{
				g_Player[0].sprite.pos.x -= g_Player[0].speed;
				g_Player[0].sprite.pos.y -= g_Player[0].speed;
				g_Player[0].sprite.facing ^= F_UP;
				g_Player[0].moving = TRUE;
			}

			if (IsButtonPressed(0, BUTTON_RIGHT))
			{
				g_Player[0].sprite.pos.y += g_Player[0].speed;
				g_Player[0].sprite.pos.x -= g_Player[0].speed;
				g_Player[0].sprite.facing ^= F_RIGHT;
				g_Player[0].moving = TRUE;
			}
			else if (IsButtonPressed(0, BUTTON_LEFT))
			{
				g_Player[0].sprite.pos.y -= g_Player[0].speed;
				g_Player[0].sprite.pos.x += g_Player[0].speed;
				g_Player[0].sprite.facing ^= F_LEFT;
				g_Player[0].moving = TRUE;
			}


		}
		else if (g_stair == 1)
		{
			if ((GetKeyboardPress(DIK_UP)) || (GetKeyboardPress(DIK_LEFT)))
			{
				g_Player[0].sprite.pos.x -= g_Player[0].speed;
				g_Player[0].sprite.pos.y -= 2.0f * g_Player[0].speed;
				g_Player[0].moving = TRUE;
				g_Player[0].sprite.facing = F_LU;
			}

			if ((GetKeyboardPress(DIK_DOWN)) || (GetKeyboardPress(DIK_RIGHT)))
			{
				g_Player[0].sprite.pos.x += g_Player[0].speed;
				g_Player[0].sprite.pos.y += 2.0f * g_Player[0].speed;
				g_Player[0].moving = TRUE;
				g_Player[0].sprite.facing = F_RD;
			}
		}
		else if (g_stair == 2)
		{
			if ((GetKeyboardPress(DIK_UP)) || (GetKeyboardPress(DIK_RIGHT)))
			{
				g_Player[0].sprite.pos.x -= 2.0f * g_Player[0].speed;
				g_Player[0].sprite.pos.y -= g_Player[0].speed;
				g_Player[0].moving = TRUE;
				g_Player[0].sprite.facing = F_RU;
			}

			if ((GetKeyboardPress(DIK_DOWN)) || (GetKeyboardPress(DIK_LEFT)))
			{
				g_Player[0].sprite.pos.x += 2.0f * g_Player[0].speed;
				g_Player[0].sprite.pos.y += g_Player[0].speed;
				g_Player[0].moving = TRUE;
				g_Player[0].sprite.facing = F_LD;
			}
		}

		//�n�`�ɂ���ăv���C���[�̈ړ��𐧌�
		MoveControl(STAGENUM, MAP_SIZE_WIDTH, MAP_SIZE_HEIGHT, GetMC(), &g_Player[0], old_pos);
		if (g_Player[0].sprite.facing == F_IDLE)
		{
			g_Player[0].sprite.facing = oldFacing;
		}
		// MAP�O�`�F�b�N
		if (g_Player[0].sprite.pos.x < 0.0f)
		{
			g_Player[0].sprite.pos.x = 0.0f;
		}

		if (g_Player[0].sprite.pos.x > MAP_SIZE_WIDTH - 0.3f)
		{
			g_Player[0].sprite.pos.x = MAP_SIZE_WIDTH - 0.3f;
		}

		if (g_Player[0].sprite.pos.y < 0.0f)
		{
			g_Player[0].sprite.pos.y = 0.0f;
		}

		if (g_Player[0].sprite.pos.y > MAP_SIZE_HEIGHT - 0.3f)
		{
			g_Player[0].sprite.pos.y = MAP_SIZE_HEIGHT - 0.3f;
		}
	}


}






void DrawPre()
{

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);
	
}




//=============================================================================
// Player�\���̂̐擪�A�h���X���擾
//=============================================================================
PLAYER* GetPlayer(void)
{
	return &g_Player[0];
}

//�v���C���[�𒆐S�ɂR���R�u���b�N�̏������ƂɁA�v���C���[�̓����𐧌䂷��
void MoveControl(int depth, int width, int height, float* MappChipList, PLAYER* player, XMFLOAT3 old_pos)
{
	float mat[3][3];
	float x, y, z;
	x = old_pos.x;
	y = old_pos.y;
	z = old_pos.z;
	int errorflag = 0;
	// �v���C���[����3x3�R�}�̏��ňړ��𐧌䂷�� 
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			mat[i + 1][j + 1] = PassCheck(depth, width, height, MappChipList, (int)z, (int)x + i, (int)y + j);
		}
	}


	if (!g_stair)
	{
		XMVECTOR Apos, Bpos, Cpos, Dpos;
		//�K�i����
		if (mat[1][0] == 0.5f)
		{
			float Ax = (int)x + 0.7f;
			float Ay = (int)y;
			float Bx = (int)x - 0.3f;
			float By = (int)y - 2.3f;
			float Cx = (int)x + 0.2f;
			float Cy = (int)y;
			float Dx = (int)x - 0.8f;
			float Dy = (int)y - 2.3f;

			Apos = XMLoadFloat3(&XMFLOAT3(Ax, Ay, 0));
			Bpos = XMLoadFloat3(&XMFLOAT3(Bx, By, 0));
			Cpos = XMLoadFloat3(&XMFLOAT3(Cx, Cy, 0));
			Dpos = XMLoadFloat3(&XMFLOAT3(Dx, Dy, 0));

			XMVECTOR playerpos = XMLoadFloat3(&player[0].sprite.pos);
			XMVECTOR AB = Bpos - Apos;
			XMVECTOR AP = playerpos - Apos;
			XMVECTOR CP = playerpos - Cpos;
			XMVECTOR CD = Dpos - Cpos;

			//�K�i�̍��E�x�N�^�[�������ĊO�όv�Z�Ńv���C���[���K�i�ɏ���Ă邩�𔻒�
			XMVECTOR Lcros = XMVector2Cross(AB, AP);
			XMVECTOR Rcros = XMVector2Cross(CD, CP);
			if ((player[0].sprite.pos.y < (int)y) && ((Lcros.m128_f32[0] * Rcros.m128_f32[0]) < 0))
			{
				g_stair = 1;
				g_stairBlock.y = (int)y;
				g_stairBlock.x = (int)x;
			}
		}

		if (PassCheck(depth, width, height, MappChipList, (int)z, (int)x + 1, (int)y + 2) == STAIRUD)
		{
			float Ax = (int)x + 1.6f;
			float Ay = (int)y + 3.0f;
			float Bx = (int)x + 0.6f;
			float By = (int)y + 0.7f;
			float Cx = (int)x + 1.0f;
			float Cy = (int)y + 3.0f;
			float Dx = (int)x;
			float Dy = (int)y + 0.7f;

			Apos = XMLoadFloat3(&XMFLOAT3(Ax, Ay, 0));
			Bpos = XMLoadFloat3(&XMFLOAT3(Bx, By, 0));
			Cpos = XMLoadFloat3(&XMFLOAT3(Cx, Cy, 0));
			Dpos = XMLoadFloat3(&XMFLOAT3(Dx, Dy, 0));

			XMVECTOR playerpos = XMLoadFloat3(&player[0].sprite.pos);
			XMVECTOR AB = Bpos - Apos;
			XMVECTOR AP = playerpos - Apos;
			XMVECTOR CP = playerpos - Cpos;
			XMVECTOR CD = Dpos - Cpos;

			//�O�όv�Z�ŊK�i������Ă邩���`�F�b�N
			XMVECTOR Lcros = XMVector2Cross(AB, AP);
			XMVECTOR Rcros = XMVector2Cross(CD, CP);
			if ((player[0].sprite.pos.y > (int)y + 0.7f) && ((Lcros.m128_f32[0] * Rcros.m128_f32[0]) < 0))
			{
				g_stair = 1;
				g_stairBlock.y = (int)y + 3;
				g_stairBlock.x = (int)x + 1;
			}
		}

		if (mat[0][1] == 0.4f)
		{
			float Ax = (int)x;
			float Ay = (int)y + 0.2f;
			float Bx = (int)x - 2.0f;
			float By = (int)y - 0.8f;
			float Cx = (int)x;
			float Cy = (int)y + 0.7f;
			float Dx = (int)x - 2.0f;
			float Dy = (int)y - 0.3f;

			Apos = XMLoadFloat3(&XMFLOAT3(Ax, Ay, 0));
			Bpos = XMLoadFloat3(&XMFLOAT3(Bx, By, 0));
			Cpos = XMLoadFloat3(&XMFLOAT3(Cx, Cy, 0));
			Dpos = XMLoadFloat3(&XMFLOAT3(Dx, Dy, 0));
			XMVECTOR playerpos = XMLoadFloat3(&player[0].sprite.pos);
			XMVECTOR AB = Bpos - Apos;
			XMVECTOR AP = playerpos - Apos;
			XMVECTOR CP = playerpos - Cpos;
			XMVECTOR CD = Dpos - Cpos;

			//�O�όv�Z�ŊK�i������Ă邩���`�F�b�N
			XMVECTOR Lcros = XMVector2Cross(AB, AP);
			XMVECTOR Rcros = XMVector2Cross(CD, CP);

			if ((player[0].sprite.pos.x < (int)x) && ((Lcros.m128_f32[0] * Rcros.m128_f32[0]) < 0))
			{
				g_stair = 2;
				g_stairBlock.x = (int)x;
				g_stairBlock.y = (int)y;
			}
		}

		if (PassCheck(depth, width, height, MappChipList, (int)z, (int)x + 2, (int)y + 1) == STAIRLR)
		{
			float Ax = (int)x + 3.0f;
			float Ay = (int)y + 1.2f;
			float Bx = (int)x + 1.0f;
			float By = (int)y + 0.2;
			float Cx = (int)x + 3.0f;
			float Cy = (int)y + 1.7f;
			float Dx = (int)x + 1.0f;
			float Dy = (int)y + 0.7f;

			Apos = XMLoadFloat3(&XMFLOAT3(Ax, Ay, 0));
			Bpos = XMLoadFloat3(&XMFLOAT3(Bx, By, 0));
			Cpos = XMLoadFloat3(&XMFLOAT3(Cx, Cy, 0));
			Dpos = XMLoadFloat3(&XMFLOAT3(Dx, Dy, 0));
			XMVECTOR playerpos = XMLoadFloat3(&player[0].sprite.pos);
			XMVECTOR AB = Bpos - Apos;
			XMVECTOR AP = playerpos - Apos;
			XMVECTOR CP = playerpos - Cpos;
			XMVECTOR CD = Dpos - Cpos;

			//�O�όv�Z�ŊK�i������Ă邩���`�F�b�N
			XMVECTOR Lcros = XMVector2Cross(AB, AP);
			XMVECTOR Rcros = XMVector2Cross(CD, CP);

			if ((player[0].sprite.pos.x > (int)x + 0.7f) && ((Lcros.m128_f32[0] * Rcros.m128_f32[0]) < 0))
			{
				g_stair = 2;
				g_stairBlock.x = (int)x + 3;
				g_stairBlock.y = (int)y + 1;
			}
		}
	}


	if (!g_stair)
	{
		if (!mat[0][0] || (mat[0][0] == STAIRUD))
		{
			if ((player[0].sprite.pos.x < (int)x) && (player[0].sprite.pos.y < (int)y))
			{

				errorflag |= 1;
			}
		}
		if ((mat[0][0] == STAIRLR))
		{
			if ((player[0].sprite.pos.x < (int)x) && (player[0].sprite.pos.y < (int)y))
			{

				errorflag |= 1;
			}
		}
		if (!mat[1][0] || (mat[1][0] == STAIRLR))
		{
			if ((player[0].sprite.pos.y < (int)y) && (player[0].sprite.pos.x > ((int)x) - 0.3f) && (player[0].sprite.pos.x < ((int)x + 1)))
			{

				errorflag |= (1 << 3);
			}
		}

		if ((mat[1][0] == STAIRUD))
		{
			if ((player[0].sprite.pos.y < (int)y) && (player[0].sprite.pos.x > ((int)x) - 0.3f) && (player[0].sprite.pos.x < ((int)x + 1)))
			{


				errorflag |= (1 << 3);
			}
		}

		if ((!mat[0][1]) || (mat[0][1] == STAIRUD))
		{
			if ((player[0].sprite.pos.x < (int)x) && (player[0].sprite.pos.y > ((int)y - 0.3f)) && (player[0].sprite.pos.y < ((int)y + 1)))
			{

				errorflag |= (1 << 1);
			}
		}

		if ((mat[0][1] == STAIRLR))
		{
			if ((player[0].sprite.pos.x < (int)x) && (player[0].sprite.pos.y > ((int)y - 0.3f)) && (player[0].sprite.pos.y < ((int)y + 1)))
			{

				errorflag |= (1 << 1);
			}
		}

		if (!mat[2][0] || (mat[2][0] == STAIRUD) || (mat[2][0] == STAIRLR))
		{
			if ((player[0].sprite.pos.x > ((int)x + 0.7f)) && (player[0].sprite.pos.y < (int)y))
			{

				errorflag |= (1 << 6);
			}
		}
		if (!mat[0][2] || (mat[0][2] == STAIRLR) || (mat[0][2] == STAIRUD))
		{
			if ((player[0].sprite.pos.y > ((int)y + 0.7f)) && (player[0].sprite.pos.x < (int)x))
			{

				errorflag |= (1 << 2);
			}
		}
		if ((!mat[2][1]) || (mat[2][1] == STAIRUD) || (mat[2][1] == STAIRLR))
		{
			if ((player[0].sprite.pos.x > ((int)x + 0.7f)) && (player[0].sprite.pos.y > ((int)y - 0.3f)) && (player[0].sprite.pos.y < ((int)y + 1)))
			{

				errorflag |= (1 << 7);
			}
		}
		if ((!mat[1][2]) || (mat[1][2] == STAIRLR) || (mat[1][2] == STAIRUD))
		{
			if ((player[0].sprite.pos.y > ((int)y + 0.7f)) && (player[0].sprite.pos.x > ((int)x - 0.3f)) && (player[0].sprite.pos.x < ((int)x + 1)))
			{

				errorflag |= (1 << 5);
			}
		}
		if ((!mat[2][2])||(mat[2][2]== STAIRLR) || (mat[2][2]== STAIRUD))
		{
			if ((player[0].sprite.pos.x > ((int)x) + 0.7f) && (player[0].sprite.pos.y > ((int)y) + 0.7f))
			{

				errorflag |= (1 << 8);
			}
		}


		if (errorflag == 0b1)
		{
			if (((int)x - player[0].sprite.pos.x) > ((int)y - player[0].sprite.pos.y))
			{
				g_Player[0].sprite.pos.y = (int)y;
			}
			else
			{
				g_Player[0].sprite.pos.x = (int)x;
			}
		}
		if (errorflag == 0b10)
		{
			g_Player[0].sprite.pos.x = (int)x;
		}
		if (errorflag == (1 << 3))
		{
			g_Player[0].sprite.pos.y = (int)y;
		}
		if (errorflag == (1 << 6))
		{

			g_Player[0].sprite.pos.y = (int)y;

		}
		if (errorflag == (1 << 2))
		{
			g_Player[0].sprite.pos.x = (int)x;
		}
		if (errorflag == (1 << 7))
		{
			g_Player[0].sprite.pos.x = (int)x + 0.7f;
		}
		if (errorflag == (1 << 5))
		{
			g_Player[0].sprite.pos.y = (int)y + 0.7f;
		}
		if (errorflag == (1 << 8))
		{
			if ((player[0].sprite.pos.x) - (int)x > (player[0].sprite.pos.y - (int)y))
			{
				g_Player[0].sprite.pos.y = (int)y + 0.7f;
			}
			else
			{
				g_Player[0].sprite.pos.x = (int)x + 0.7f;
			}
		}
		if (errorflag == 0b11)
		{
			g_Player[0].sprite.pos.x = (int)x;
		}
		if (errorflag == 0b1001)
		{
			g_Player[0].sprite.pos.y = (int)y;
		}
		if (errorflag == ((1 << 1) | (1 << 3)))
		{
			g_Player[0].sprite.pos.x = (int)x;
			g_Player[0].sprite.pos.y = (int)y;
		}
		if (errorflag == ((1 << 1) | (1 << 5)))
		{
			g_Player[0].sprite.pos.x = (int)x;
			g_Player[0].sprite.pos.y = (int)y + 0.7f;
		}
		if (errorflag == ((1 << 7) | (1 << 3)))
		{
			g_Player[0].sprite.pos.x = (int)x + 0.7f;
			g_Player[0].sprite.pos.y = (int)y;
		}
		if (errorflag == ((1 << 7) | (1 << 5)))
		{
			g_Player[0].sprite.pos.x = (int)x + 0.7f;
			g_Player[0].sprite.pos.y = (int)y + 0.7f;
		}
		if (errorflag == 0b1011)
		{
			g_Player[0].sprite.pos.x = (int)x;
			g_Player[0].sprite.pos.y = (int)y;
		}
		if (errorflag == ((1 << 6) | (1 << 3)))
		{
			g_Player[0].sprite.pos.y = (int)y;
		}
		if (errorflag == ((1 << 6) | (1 << 7)))
		{
			g_Player[0].sprite.pos.x = (int)x + 0.7f;
		}
		if (errorflag == ((1 << 1) | (1 << 2)))
		{
			g_Player[0].sprite.pos.x = (int)x;
		}
		if (errorflag == ((1 << 5) | (1 << 2)))
		{
			g_Player[0].sprite.pos.y = (int)y + 0.7f;
		}
		if (errorflag == ((1 << 7) | (1 << 8)))
		{
			g_Player[0].sprite.pos.x = (int)x + 0.7f;
		}
		if (errorflag == ((1 << 5) | (1 << 8)))
		{
			g_Player[0].sprite.pos.y = (int)y + 0.7f;
		}
		if (errorflag == ((1 << 5) | (1 << 8) | (1 << 7)))
		{
			g_Player[0].sprite.pos.x = (int)x + 0.7f;
			g_Player[0].sprite.pos.y = (int)y + 0.7f;
		}
		if (errorflag == ((1 << 7) | (1 << 3) | (1 << 6)))
		{
			g_Player[0].sprite.pos.x = (int)x + 0.7f;
			g_Player[0].sprite.pos.y = (int)y;
		}
		if (errorflag == ((1 << 1) | (1 << 2) | (1 << 5)))
		{
			g_Player[0].sprite.pos.x = (int)x;
			g_Player[0].sprite.pos.y = (int)y + 0.7f;
		}

	}

	if (g_stair == 1)
	{
		if ((!PassCheck(depth, width, height, MappChipList, 0, (int)g_stairBlock.x - 1, g_stairBlock.y - 3)) || (PassCheck(depth, width, height, MappChipList, 0, g_stairBlock.x - 1, g_stairBlock.y - 3) == STAIRLR))
		{
			if ((player[0].sprite.pos.y < g_stairBlock.y - 2.0f) && (player[0].sprite.facing == F_LU))
			{
				player[0].sprite.pos = old_pos;
			}
		}
		if ((!PassCheck(depth, width, height, MappChipList, 0, (int)g_stairBlock.x, g_stairBlock.y - 2))&&((int)g_stairBlock.y - 1==0))
		{
			if ((player[0].sprite.pos.y < g_stairBlock.y - 1.0f) && (player[0].sprite.facing == F_LU))
			{
				player[0].sprite.pos = old_pos;
			}
		}

		if (!PassCheck(depth, width, height, MappChipList, 0, (int)g_stairBlock.x, g_stairBlock.y) || (PassCheck(depth, width, height, MappChipList, 0, g_stairBlock.x, g_stairBlock.y) == STAIRLR))
		{
			if ((player[0].sprite.pos.y > (g_stairBlock.y - 0.3f)) && (player[0].sprite.facing == F_RD) && (PassCheck(depth, width, height, MappChipList, 0, g_stairBlock.x + 1, g_stairBlock.y + 1) != STAIRUD))
			{
				player[0].sprite.pos = old_pos;
			}
		}
	}

	if (g_stair == 2)
	{
		if (!PassCheck(depth, width, height, MappChipList, 0, (int)g_stairBlock.x - 3, g_stairBlock.y - 1) || (PassCheck(depth, width, height, MappChipList, 0, g_stairBlock.x - 3, g_stairBlock.y - 1) == STAIRUD))

		{
			if ((player[0].sprite.pos.x < (g_stairBlock.x - 2.0f)) && (player[0].sprite.facing == F_RU))
			{
				player[0].sprite.pos = old_pos;
			}
		}
		if ((!PassCheck(depth, width, height, MappChipList, 0, (int)g_stairBlock.x - 2, g_stairBlock.y))&&((int)g_stairBlock.x - 1==0))
		{
			if ((player[0].sprite.pos.x < (g_stairBlock.x - 1.0f)) && (player[0].sprite.facing == F_RU))
			{
				player[0].sprite.pos = old_pos;
			}
		}
		if (!PassCheck(depth, width, height, MappChipList, 0, (int)g_stairBlock.x, g_stairBlock.y) || (PassCheck(depth, width, height, MappChipList, 0, g_stairBlock.x, g_stairBlock.y) == STAIRUD))
		{
			if ((player[0].sprite.pos.x > (g_stairBlock.x - 0.3f)) && (player[0].sprite.facing == F_LD)&&(PassCheck(depth, width, height, MappChipList, 0, g_stairBlock.x+1, g_stairBlock.y+1)!= STAIRLR))
			{
				player[0].sprite.pos = old_pos;
			}
		}
	}

	if (g_stair == 1)
	{
		if (player[0].sprite.facing == F_LU)
		{
			if ((player[0].sprite.pos.y < (g_stairBlock.y - 2.3f)))
			{
				if ((PassCheck(depth, width, height, MappChipList, 0, (int)player[0].sprite.pos.x, (int)player[0].sprite.pos.y) == STAIRUD))
				{
					
					g_stairBlock.x--;
					g_stairBlock.y -= 2;
				}
				else
				{
					g_stair = 0;
				}

			}
		}

		if (player[0].sprite.facing == F_RD)
		{
			if ((player[0].sprite.pos.y >= g_stairBlock.y))
			{
				if ((PassCheck(depth, width, height, MappChipList, 0, (int)player[0].sprite.pos.x+1, (int)player[0].sprite.pos.y+1) == STAIRUD))
				{
					g_stairBlock.x++;
					g_stairBlock.y += 2;
				}
				else
				{
					g_stair = 0;
				}

			}
		}


	}

	if (g_stair == 2)
	{
		if (player[0].sprite.facing == F_RU)
		{
			if (((player[0].sprite.pos.x < (g_stairBlock.x - 2.3f))))
			{
				if ((PassCheck(depth, width, height, MappChipList, 0, (int)player[0].sprite.pos.x, (int)player[0].sprite.pos.y) == STAIRLR)) //(GetMC()[(int)player[0].sprite.pos.x * MAP_SIZE_WIDTH + (int)player[0].sprite.pos.y] == 0.4f)
				{
					g_stairBlock.x -= 2;
					g_stairBlock.y--;
				}
				else
				{
					g_stair = 0;
				}
			}
		}

		if ((player[0].sprite.facing == F_LD))
		{
			if (player[0].sprite.pos.x >= g_stairBlock.x)
			{
				if ((PassCheck(depth, width, height, MappChipList, 0, (int)player[0].sprite.pos.x +1, (int)player[0].sprite.pos.y +1) == STAIRLR))//(GetMC()[((int)player[0].sprite.pos.x + 1) * MAP_SIZE_WIDTH + (int)player[0].sprite.pos.y + 1] == 0.4f)
				{
					g_stairBlock.x += 2;
					g_stairBlock.y++;
				}
				else
				{
					g_stair = 0;
				}
			}

		}

	}



}

int GetStair(void)
{
	return g_stair;
}

StairStats* GetStairBlock(void)
{
	return &g_stairBlock;
}
void ClearStair(void)
{
	g_stair = 0;
	g_stairBlock.x = 0;
	g_stairBlock.y = 0;
}