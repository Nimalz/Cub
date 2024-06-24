
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
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(100/2)	// キャラサイズ
#define TEXTURE_HEIGHT				(100/2)	// 
#define TEXTURE_MAX					(2)		// テクスチャの数

#define TEXTURE_PATTERN_DIVIDE_X	(4)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数
#define ANIM_WAIT					(4)		// アニメーションの切り替わるWait値
#define TIME_PAUSE_SIZE				(2000.0f)	// timepause texture size in pixel
#define TIME_PAUSE_SPEED			(50.0f)		// timepause texture size 拡大速度　pixel　per frame
#define PAUSE_ANI_FRAMES			(TIME_PAUSE_SIZE/TIME_PAUSE_SPEED)	//pause　アニメション　総フレーム数
#define BGM_SLOW_RATE				(1/TIME_PAUSE_SIZE)				//時間静止　アニメションに合わせて　BGMの再生速度を落とす速度
#define AIM_SPPED					(0.05f)


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char* g_TexturName[] = {
	"data/TEXTURE/shadow000.jpg",
	"data/TEXTURE/aim2.png",
};

static BOOL		g_Load = FALSE;			 // 初期化を行ったかのフラグ
static BULLET	g_Bullet[BULLET_MAX];	 // バレット構造体
static float    g_stopCount;			 // 地形移動モードアニメイシ用

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBullet(void)
{
	ID3D11Device* pDevice = GetDevice();

	//テクスチャ生成
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


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// バレット構造体の初期化
	for (int i = 0; i < BULLET_MAX; i++)
	{
		g_Bullet[i].use = FALSE;			// 未使用（発射されていない弾）
		g_Bullet[i].w = TEXTURE_WIDTH;
		g_Bullet[i].h = TEXTURE_HEIGHT;
		g_Bullet[i].pos = XMFLOAT3(2.0f, 2.0f, 0.0f);
		g_Bullet[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Bullet[i].texNo = 0;

		g_Bullet[i].countAnim = 0;
		g_Bullet[i].patternAnim = 0;

		//g_Bullet[i].move = XMFLOAT3(0.0f, -BULLET_SPEED, 0.0f);	// 移動量を初期化
		g_Bullet[i].move = XMFLOAT3(0.0f, 0.0F, 0.0f);
		g_Bullet[i].BezierT = 0.0f;
	}
	g_stopCount = 0.0f;
	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
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
// 更新処理
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
			//地形移動モードに入る
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
	// アニメーション  
	if (g_Bullet[0].use)
	{
		g_stopCount += TIME_PAUSE_SPEED;
		if (g_stopCount > TIME_PAUSE_SIZE)
			g_stopCount = TIME_PAUSE_SIZE;
		g_Bullet[0].countAnim++;
		if (g_Bullet[0].countAnim > ANIM_WAIT)
		{
			g_Bullet[0].countAnim = 0;
			// パターンの切り替え
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



		// MAP外チェック
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
// 描画処理
//=============================================================================
void DrawBullet(void)
{

		// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//マテリアル設定
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

	//照準カーソルの位置やテクスチャー座標を反映
	float px = DEFAULT_X - g_Bullet[0].pos.x * MAPCHIP_WIDTH * MAP_SIZE_MOD * 0.5f + g_Bullet[0].pos.y * MAPCHIP_WIDTH * MAP_SIZE_MOD * 0.5f;
	float py = DEFAULT_Y + g_Bullet[0].pos.x * MAPCHIP_WIDTH * MAP_SIZE_MOD * 0.25f + g_Bullet[0].pos.y * MAPCHIP_WIDTH * MAP_SIZE_MOD * 0.25f - 0.25f * MAPCHIP_WIDTH * MAP_SIZE_MOD;
	float pw = g_Bullet[0].w;		// プレイヤーの表示幅
	float ph = g_Bullet[0].h;		// プレイヤーの表示高さ

	// アニメーション用
	float tw = 1.0f / (TEXTURE_PATTERN_DIVIDE_X)-0.002f;	// テクスチャの幅
	float th = 1.0f;	// テクスチャの高さ
	float tx = (float)(g_Bullet[0].patternAnim % TEXTURE_PATTERN_DIVIDE_X) * tw - 0.002f;                      	// テクスチャの左上X座標
	float ty = 0.0f;	// テクスチャの左上Y座標



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
	//照準カーソルの位置やテクスチャー座標を反映
	float px = DEFAULT_X - player[0].sprite.pos.x * MAPCHIP_WIDTH * MAP_SIZE_MOD * 0.5f + player[0].sprite.pos.y * MAPCHIP_WIDTH * MAP_SIZE_MOD * 0.5f;
	float py = DEFAULT_Y + player[0].sprite.pos.x * MAPCHIP_WIDTH * MAP_SIZE_MOD * 0.25f + player[0].sprite.pos.y * MAPCHIP_WIDTH * MAP_SIZE_MOD * 0.25f - 0.25f * MAPCHIP_WIDTH * MAP_SIZE_MOD;
	float pw = g_stopCount;		// プレイヤーの表示幅
	float ph = g_stopCount;		// プレイヤーの表示高さ

	// アニメーション用
	float tw = 1.0f;	// テクスチャの幅
	float th = 1.0f;	// テクスチャの高さ
	float tx = 0.0f;                      	// テクスチャの左上X座標
	float ty = 0.0f;	// テクスチャの左上Y座標


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
// バレット構造体の先頭アドレスを取得
//=============================================================================
BULLET* GetBullet(void)
{
	return &g_Bullet[0];
}


//=============================================================================
// バレットの発射設定
//=============================================================================.

void SetBullet(XMFLOAT3 pos)
{
	// もし未使用の弾が無かったら発射しない( =これ以上撃てないって事 )
	for (int i = 0; i < BULLET_MAX; i++)
	{
		if (g_Bullet[i].use == FALSE)		// 未使用状態のバレットを見つける
		{
			g_Bullet[i].use = TRUE;			// 使用状態へ変更する
			g_Bullet[i].pos = pos;			// 座標をセット
			g_Bullet[i].BezierT = 0.0f;
			return;							// 1発セットしたので終了する
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

