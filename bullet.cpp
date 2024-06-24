
#include "bullet.h"
#include "enemy.h"
#include "collision.h"
#include "score.h"
#include "player.h"
#include "input.h"
#include "field.h"
#include "sound.h"
#include "showInput.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(100/2)	// �L�����T�C�Y
#define TEXTURE_HEIGHT				(100/2)	// 
#define TEXTURE_MAX					(2)		// �e�N�X�`���̐�

#define TEXTURE_PATTERN_DIVIDE_X	(4)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define ANIM_WAIT					(4)		// �A�j���[�V�����̐؂�ւ��Wait�l
#define TIME_PAUSE_SIZE				(2000.0f)	// timepause texture size in pixel
#define TIME_PAUSE_SPEED			(50.0f)		// timepause texture size �g�呬�x�@pixel�@per frame
#define PAUSE_ANI_FRAMES			(TIME_PAUSE_SIZE/TIME_PAUSE_SPEED)	//pause�@�A�j���V�����@���t���[����
#define BGM_SLOW_RATE				(1/TIME_PAUSE_SIZE)				//���ԐÎ~�@�A�j���V�����ɍ��킹�ā@BGM�̍Đ����x�𗎂Ƃ����x
#define AIM_SPPED					(0.05f)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char* g_TexturName[] = {
	"data/TEXTURE/shadow000.jpg",
	"data/TEXTURE/aim2.png",
};

static BOOL		g_Load = FALSE;			 // ���������s�������̃t���O
static BULLET	g_Bullet[BULLET_MAX];	 // �o���b�g�\����
static float    g_stopCount;			 // �n�`�ړ����[�h�A�j���C�V�p

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBullet(void)
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


	// �o���b�g�\���̂̏�����
	for (int i = 0; i < BULLET_MAX; i++)
	{
		g_Bullet[i].use = FALSE;			// ���g�p�i���˂���Ă��Ȃ��e�j
		g_Bullet[i].w = TEXTURE_WIDTH;
		g_Bullet[i].h = TEXTURE_HEIGHT;
		g_Bullet[i].pos = XMFLOAT3(2.0f, 2.0f, 0.0f);
		g_Bullet[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Bullet[i].texNo = 0;

		g_Bullet[i].countAnim = 0;
		g_Bullet[i].patternAnim = 0;

		//g_Bullet[i].move = XMFLOAT3(0.0f, -BULLET_SPEED, 0.0f);	// �ړ��ʂ�������
		g_Bullet[i].move = XMFLOAT3(0.0f, 0.0F, 0.0f);
		g_Bullet[i].BezierT = 0.0f;
	}
	g_stopCount = 0.0f;
	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBullet(void)
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

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBullet(void)
{

	if (!GetSelect())
		UpdateAim();

}

void UpdateAim(void)
{
	if (GetKeyboardTrigger(DIK_E)&& !GetMenuFlag())
	{
		if (GetBullet()->use == FALSE)
		{
			GetInputStates()->T_ABI1 = TRUE;
			//�n�`�ړ����[�h�ɓ���
			GetBullet()->use = TRUE;
			GetBullet()->pos = GetPlayer()[0].sprite.pos;
			GetPlayer()[0].use = FALSE;
			return;
		}
		else
		{
			GetBullet()->use = FALSE;
			GetPlayer()[0].use = TRUE;
		}

	}
	if (!g_Bullet[0].use)
	{
		g_stopCount -= TIME_PAUSE_SPEED;
		if (g_stopCount < 0.0f)
			g_stopCount = 0.0f;
	}
	// �A�j���[�V����  
	if (g_Bullet[0].use)
	{
		g_stopCount += TIME_PAUSE_SPEED;
		if (g_stopCount > TIME_PAUSE_SIZE)
			g_stopCount = TIME_PAUSE_SIZE;
		g_Bullet[0].countAnim++;
		if (g_Bullet[0].countAnim > ANIM_WAIT)
		{
			g_Bullet[0].countAnim = 0;
			// �p�^�[���̐؂�ւ�
			g_Bullet[0].patternAnim = (g_Bullet[0].patternAnim + 1) % ANIM_PATTERN_NUM;
		}

		if (GetKeyboardPress(DIK_RIGHT))
		{
			GetInputStates()->T_RIGHT = TRUE;
			g_Bullet[0].pos.y += AIM_SPPED;
			g_Bullet[0].pos.x -= AIM_SPPED;
		}
		else if (GetKeyboardPress(DIK_LEFT))
		{
			GetInputStates()->T_LEFT = TRUE;
			g_Bullet[0].pos.y -= AIM_SPPED;
			g_Bullet[0].pos.x += AIM_SPPED;
		}

		if (GetKeyboardPress(DIK_DOWN))
		{
			GetInputStates()->T_DOWN = TRUE;
			g_Bullet[0].pos.x += AIM_SPPED;
			g_Bullet[0].pos.y += AIM_SPPED;
		}
		else if (GetKeyboardPress(DIK_UP))
		{
			GetInputStates()->T_UP = TRUE;
			g_Bullet[0].pos.x -= AIM_SPPED;
			g_Bullet[0].pos.y -= AIM_SPPED;
		}



		// MAP�O�`�F�b�N
		if (g_Bullet[0].pos.x < 0.0f)
		{
			g_Bullet[0].pos.x = 0.0f;
		}

		if (g_Bullet[0].pos.x > MAP_SIZE_WIDTH - 0.3f)
		{
			g_Bullet[0].pos.x = MAP_SIZE_WIDTH - 0.3f;
		}

		if (g_Bullet[0].pos.y < 0.0f)
		{
			g_Bullet[0].pos.y = 0.0f;
		}

		if (g_Bullet[0].pos.y > MAP_SIZE_HEIGHT - 0.3f)
		{
			g_Bullet[0].pos.y = MAP_SIZE_HEIGHT - 0.3f;
		}
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void DrawBullet(void)
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

	if (g_Bullet[0].use == TRUE)
	{
		if (!GetSelect())
			DrawAim();
		
	}
	
	DrawTimeStop();
}

void DrawAim(void)
{

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

	//�Ə��J�[�\���̈ʒu��e�N�X�`���[���W�𔽉f
	float px = DEFAULT_X - g_Bullet[0].pos.x * MAPCHIP_WIDTH * MAP_SIZE_MOD * 0.5f + g_Bullet[0].pos.y * MAPCHIP_WIDTH * MAP_SIZE_MOD * 0.5f;
	float py = DEFAULT_Y + g_Bullet[0].pos.x * MAPCHIP_WIDTH * MAP_SIZE_MOD * 0.25f + g_Bullet[0].pos.y * MAPCHIP_WIDTH * MAP_SIZE_MOD * 0.25f - 0.25f * MAPCHIP_WIDTH * MAP_SIZE_MOD;
	float pw = g_Bullet[0].w;		// �v���C���[�̕\����
	float ph = g_Bullet[0].h;		// �v���C���[�̕\������

	// �A�j���[�V�����p
	float tw = 1.0f / (TEXTURE_PATTERN_DIVIDE_X)-0.002f;	// �e�N�X�`���̕�
	float th = 1.0f;	// �e�N�X�`���̍���
	float tx = (float)(g_Bullet[0].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw - 0.002f;                      	// �e�N�X�`���̍���X���W
	float ty = 0.0f;	// �e�N�X�`���̍���Y���W



	SetSpriteColorRotationBottomUp(g_VertexBuffer, px, py, pw, ph,
		tx, ty, tw, th,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		g_Bullet[0].rot.z);
	GetDeviceContext()->Draw(4, 0);

}

void DrawTimeStop(void)
{
	if ((g_stopCount>0.0f)&&(g_stopCount<=2000.0f))
		Sampleratedown(/*GetStageInfo()->bgm*/SOUND_LABEL_BGM_sample001, 1.0f-BGM_SLOW_RATE* g_stopCount);
	SetBlendState(BLEND_MODE_SUBTRACT);
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);
	PLAYER* player = GetPlayer();
	//�Ə��J�[�\���̈ʒu��e�N�X�`���[���W�𔽉f
	float px = DEFAULT_X - player[0].sprite.pos.x * MAPCHIP_WIDTH * MAP_SIZE_MOD * 0.5f + player[0].sprite.pos.y * MAPCHIP_WIDTH * MAP_SIZE_MOD * 0.5f;
	float py = DEFAULT_Y + player[0].sprite.pos.x * MAPCHIP_WIDTH * MAP_SIZE_MOD * 0.25f + player[0].sprite.pos.y * MAPCHIP_WIDTH * MAP_SIZE_MOD * 0.25f - 0.25f * MAPCHIP_WIDTH * MAP_SIZE_MOD;
	float pw = g_stopCount;		// �v���C���[�̕\����
	float ph = g_stopCount;		// �v���C���[�̕\������

	// �A�j���[�V�����p
	float tw = 1.0f;	// �e�N�X�`���̕�
	float th = 1.0f;	// �e�N�X�`���̍���
	float tx = 0.0f;                      	// �e�N�X�`���̍���X���W
	float ty = 0.0f;	// �e�N�X�`���̍���Y���W


	for (int i = 0; i < 1; i++)
	{
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph,
			tx, ty, tw, th,
			XMFLOAT4(0.17f, 0.3f, 0.45f, 0.3f));
		GetDeviceContext()->Draw(4, 0);
	}




	SetBlendState(BLEND_MODE_ALPHABLEND);

}
//=============================================================================
// �o���b�g�\���̂̐擪�A�h���X���擾
//=============================================================================
BULLET* GetBullet(void)
{
	return &g_Bullet[0];
}


//=============================================================================
// �o���b�g�̔��ːݒ�
//=============================================================================.

void SetBullet(XMFLOAT3 pos)
{
	// �������g�p�̒e�����������甭�˂��Ȃ�( =����ȏ㌂�ĂȂ����Ď� )
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Bullet[i].use == FALSE)		// ���g�p��Ԃ̃o���b�g��������
		{
			g_Bullet[i].use = TRUE;			// �g�p��Ԃ֕ύX����
			g_Bullet[i].pos = pos;			// ���W���Z�b�g
			g_Bullet[i].BezierT = 0.0f;
			return;							// 1���Z�b�g�����̂ŏI������
		}
	}
}

XMVECTOR QuadraticBezierCurve(XMVECTOR p0, XMVECTOR p1, XMVECTOR p2, XMVECTOR p3, float s)
{
	XMVECTOR B01 = LinearInterpolation(p0, p1, s);
	XMVECTOR B23 = LinearInterpolation(p2, p3, s);
	XMVECTOR B12 = LinearInterpolation(p1, p2, s);
	XMVECTOR B0112 = LinearInterpolation(B01, B12, s);
	XMVECTOR B1223 = LinearInterpolation(B12, B23, s);
	return  LinearInterpolation(B0112, B1223, s);
}

