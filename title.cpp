
#include "title.h"
#include "input.h"
#include "fade.h"
#include "text.h"
#include "sound.h"
#include "file.h"
#include "field.h"
#include "option.h"
#include "field.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(17)				// テクスチャの数

#define TEXTURE_BG_SIZE             (1470)			//　背景サイズ
#define TETURE_EARTH_SIZE1			(200)			//　地球サイズ（小）
#define EARTH_FRAME					(109)			//	地球テクスチャフレイム数
#define MODDER						(4)				// タイトル画面選択可能な指令数（0:new game 1:continue 2:option 4:exit）
#define ANI_AWAIT					(8)				// アニメーションの切り替わるWait値
#define RGB_GRAY					(float)(127.0f / 255.0f)
#define BUTTON_ANI_FRAME			(15)

enum PHASE
{
	IDLE,
	SHIFT,
	LOGO,
	MENU,

	PHASE_MAX,

};
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/Large 1024x1024/Starfields/Starfield_05-1024x1024.png",
	"data/TEXTURE/3429403387.png",
	"data/TEXTURE/LOGO.png",
	"data/TEXTURE/continue.png",
	"data/TEXTURE/option.png",
	"data/TEXTURE/exit.png",
	"data/TEXTURE/Newgame2.png",
	"data/TEXTURE/option2.png",
	"data/TEXTURE/exit2.png",
	"data/TEXTURE/cub.png",
	"data/TEXTURE/UI_BG.png",
	"data/TEXTURE/slide1.png",
	"data/TEXTURE/slide2.png",
	"data/TEXTURE/ball.png",
	"data/TEXTURE/white.png",
	"data/TEXTURE/Keyboard Extras.png",
	"data/TEXTURE/Keyboard Letters and Symbols.png",
};


static BOOL						g_Use;						// TRUE:使っている  FALSE:未使用
static float					g_w, g_h;					// 幅と高さ
static XMFLOAT3					g_Pos;						// ポリゴンの座標
static int						g_TexNo;					// テクスチャ番号
static int						g_Pattern = 0.0f;

static int						g_phase;					//フェイス保存
static BOOL						g_Load = FALSE;
static float					g_scrl;						//BGスケール用
static float					g_shift = 0.0f;				//BGすらす用
static int						g_cursor = 0;				//カーソル（0:new game 1:continue 2:option 3:exit）

//static BOOL						g_option_flag = FALSE;		//option　flag
//static int						g_option_cursor = 0;		//Option内部カーソル
//
static int						g_arrowcnt1 = 0;			//アニメイシ用
static int						g_arrowcnt2 = 0;			//アニメイシ用
static int						g_arrowcnt3 = 0;			//アニメイシ用
//static int						g_arrowcnt4 = 0;			//アニメイシ用
//static int						g_arrowcnt5 = 0;			//アニメイシ用
static BOOL                     g_return = FALSE;
static BOOL                     g_return2 = FALSE;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitle(void)
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


	// 変数の初期化
	g_Use = TRUE;
	g_w = TEXTURE_WIDTH;
	g_h = TEXTURE_HEIGHT;
	g_Pos = XMFLOAT3(g_w / 2, g_h / 2, 0.0f);
	g_TexNo = 0;


	g_Load = TRUE;
	g_Pattern = 0;
	g_phase = IDLE;
	g_scrl = 0.0f;
	g_shift = 0.0f;
	if (!GetLoadCheck())
		g_cursor = 0;
	if (g_return)
	{
		g_cursor = 1;

	}
	if (g_return2)
	{
		g_phase = MENU;
	}
	g_arrowcnt1 = 0;
	g_arrowcnt2 = 0;
	g_arrowcnt3 = 0;
	//g_option_flag = FALSE;
	//g_option_cursor = 0;

	//g_arrowcnt4 = 0;
	//g_arrowcnt5 = 0;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
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
// 更新処理
//=============================================================================
void UpdateTitle(void)
{
	if (*GetStageClear() < 0)
		*GetStageClear() = STAGENUM - 1;


	switch (g_phase)
	{
	case IDLE:
		if (GetKeyboardTrigger(DIK_RETURN))
		{// Enter押したら、ステージを切り替える
			g_phase++;
		}
		// ゲームパッドで入力処理
		else if (IsButtonTriggered(0, BUTTON_START))
		{
			g_phase++;
		}
		else if (IsButtonTriggered(0, BUTTON_B))
		{
			g_phase++;
		}
		break;
	case SHIFT:
		g_shift++;
		break;
	case LOGO:
		if (GetKeyboardTrigger(DIK_RETURN))
		{// Enter押したら、ステージを切り替える
			PlaySound(SOUND_LABEL_SE_confirm);
			g_phase++;
		}
		// ゲームパッドで入力処理
		else if (IsButtonTriggered(0, BUTTON_START))
		{
			PlaySound(SOUND_LABEL_SE_confirm);
			g_phase++;
		}
		else if (IsButtonTriggered(0, BUTTON_B))
		{
			PlaySound(SOUND_LABEL_SE_confirm);
			g_phase++;
		}
		break;
	case MENU:
		if (!*GetOptionFlag())
		{
			if (!g_arrowcnt3)
			{
				if (GetKeyboardTrigger(DIK_LEFT))
				{
					g_arrowcnt1 = BUTTON_ANI_FRAME;
					g_cursor--;
					g_cursor %= MODDER;
					PlaySound(SOUND_LABEL_SE_menu1);
				}

				else if (GetKeyboardTrigger(DIK_RIGHT))
				{
					g_arrowcnt2 = BUTTON_ANI_FRAME;
					g_cursor++;
					g_cursor %= MODDER;
					PlaySound(SOUND_LABEL_SE_menu1);
				}
			}


			if ((GetKeyboardTrigger(DIK_RETURN)))
			{
				if (!g_arrowcnt3)
				{

					if ((*GetStageClear() == 0) && ((g_cursor == -3) || (g_cursor == 1)))
					{

						PlaySound(SOUND_LABEL_SE_Deny);
					}
					else
					{
						g_arrowcnt3 = BUTTON_ANI_FRAME * 2.0f;
						PlaySound(SOUND_LABEL_SE_confirm);

					}

				}
				/*	if ((g_cursor == 2) || (g_cursor == -2))
					{
						*GetOptionFlag() = TRUE;
					}*/

			}
			if (g_cursor == 0)
			{
				*GetStage() = 0;
			}


		}
		/*	else
			{

				UpdateOption();
			}*/

		break;
	}

}
//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);
	SetBlendState(BLEND_MODE_ALPHABLEND);
	// タイトルの背景を描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColorRotation(g_VertexBuffer, SCREEN_CENTER_X, SCREEN_CENTER_Y, TEXTURE_BG_SIZE + g_shift / 4, TEXTURE_BG_SIZE + g_shift / 4, g_scrl, 0.0f, 1.0f, 1.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), g_scrl);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
		g_scrl += -0.00004f;//背景スクロール速度
	}



	switch (g_phase)
	{
	case IDLE:
	{
		static int textcon = 0;
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定

		SetSprite(g_VertexBuffer, SCREEN_CENTER_X, SCREEN_CENTER_Y, TETURE_EARTH_SIZE1, TETURE_EARTH_SIZE1, 1.0f / 109 * (int)(g_Pattern / 8), 0.0f, 1.0f / 109, 1.0f);
		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
		g_Pattern++;
		g_Pattern %= EARTH_FRAME * ANI_AWAIT;
		textcon++;
		textcon %= 200;
		if (textcon < 150)
			IdleText();
	}

	break;
	case SHIFT:
		//SetBlendState(BLEND_MODE_ADD);
		if (g_shift <= 300.0f)
		{

			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);


			SetSprite(g_VertexBuffer, SCREEN_CENTER_X, SCREEN_CENTER_Y, 200 + g_shift, 200 + g_shift, 1.0f / 109 * (int)(g_Pattern / 8), 0.0f, 1.0f / 109, 1.0f);

			// ポリゴン描画

			GetDeviceContext()->Draw(4, 0);
			g_Pattern++;
			g_Pattern %= EARTH_FRAME * ANI_AWAIT;
			g_shift += 1.0f;

		}
		else
			g_phase = LOGO;
		//SetBlendState(BLEND_MODE_ALPHABLEND);
		break;
	case LOGO:

	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定

		SetSprite(g_VertexBuffer, SCREEN_CENTER_X, SCREEN_CENTER_Y, 500, 500, 1.0f / 109 * (int)(g_Pattern / 8), 0.0f, 1.0f / 109, 1.0f);
		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
		g_Pattern++;
		g_Pattern %= EARTH_FRAME * ANI_AWAIT;

	}
	//ロゴを描画

	{
		static float trans = 0.0f;
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定

		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X - 15, SCREEN_CENTER_Y - 50, 1280 * 1.3f, 720, 1.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, trans));
		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[9]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定


		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X, SCREEN_CENTER_Y + 200, 744, 502, 1.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, trans));
		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
		//LogoText1();
		if (trans < 0.95f)
		{
			if (trans > 0.5f)
				trans += 0.01f;
			else
				trans += 0.004f;
		}
		else
		{
			static int textcon = 0.0f;
			//LogoText1();
			//LogoText2();
			textcon++;
			textcon %= 500;
			if (textcon < 350)
				LogoText3();
			else
				IdleText();
		}


	}

	break;
	case MENU:

		if (!*GetOptionFlag())
			MenuText1();
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

			// １枚のポリゴンの頂点とテクスチャ座標を設定

			SetSprite(g_VertexBuffer, SCREEN_CENTER_X, SCREEN_CENTER_Y, 500, 500, 1.0f / 109 * (int)(g_Pattern / 8), 0.0f, 1.0f / 109, 1.0f);
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
			g_Pattern++;
			g_Pattern %= EARTH_FRAME * ANI_AWAIT;

		}

		{
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

			// １枚のポリゴンの頂点とテクスチャ座標を設定

			SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X - 15, SCREEN_CENTER_Y - 50, 1280 * 1.3f, 720, 1.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);

		}
		//NEW GAME
		if (g_cursor == 0)
		{
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[6]);

			// １枚のポリゴンの頂点とテクスチャ座標を設定

			SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X + 29, SCREEN_CENTER_Y + 230, 696, 564, 1.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);

			if (g_arrowcnt3)
			{

				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[6]);

				// １枚のポリゴンの頂点とテクスチャ座標を設定

				SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X + 29, SCREEN_CENTER_Y + 230, 696, 564, 1.0f, 0.0f, 1.0f, 1.0f,
					XMFLOAT4(1.0f, 0.5f, 0.5f, 0.7f));
				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);
				g_arrowcnt3--;
				if (!g_arrowcnt3)
					SetFade(FADE_OUT, MODE_GAME);
			}
		}
		//CONTINUE
		else if ((g_cursor == -3) || (g_cursor == 1))
		{
			if (*GetStageClear() == 0)
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);
				SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X + 23, SCREEN_CENTER_Y + 230, 696, 564, 1.0f, 0.0f, 1.0f, 1.0f,
					XMFLOAT4(RGB_GRAY, RGB_GRAY, RGB_GRAY, 1.0f));
				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);

			}
			else {
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);
				SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X + 23, SCREEN_CENTER_Y + 230, 696, 564, 1.0f, 0.0f, 1.0f, 1.0f,
					XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);
				if (g_arrowcnt3)
				{

					GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);

					// １枚のポリゴンの頂点とテクスチャ座標を設定

					SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X + 23, SCREEN_CENTER_Y + 230, 696, 564, 1.0f, 0.0f, 1.0f, 1.0f,
						XMFLOAT4(1.0f, 0.5f, 0.5f, 0.7f));
					// ポリゴン描画
					GetDeviceContext()->Draw(4, 0);
					g_arrowcnt3--;
					if (!g_arrowcnt3)

					{
						*GetStage() = *GetStageClear();
						SetFade(FADE_OUT, MODE_GAME);

					}
				}
			}


		}
		else if ((g_cursor == -2) || (g_cursor == 2))
		{
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[7]);

			// １枚のポリゴンの頂点とテクスチャ座標を設定

			SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X + 23, SCREEN_CENTER_Y + 230, 696, 564, 1.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);

			if (g_arrowcnt3)
			{

				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[7]);

				// １枚のポリゴンの頂点とテクスチャ座標を設定

				SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X + 23, SCREEN_CENTER_Y + 230, 696, 564, 1.0f, 0.0f, 1.0f, 1.0f,
					XMFLOAT4(1.0f, 0.5f, 0.5f, 0.7f));
				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);
				g_arrowcnt3--;
				if (!g_arrowcnt3)

				{
					*GetOptionFlag() = TRUE;

				}
			}
			/*		if (*GetOptionFlag())
					{
						DrawOption();
						DrawVolumeMas();
						DrawVolumeBGM();
						DrawVolumeSE();
						DrawOptionSelect();
						DrawOptionKey();

					}*/
		}
		else if ((g_cursor == -1) || (g_cursor == 3))
		{
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[8]);

			// １枚のポリゴンの頂点とテクスチャ座標を設定

			SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X + 17, SCREEN_CENTER_Y + 230, 696, 564, 1.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
			if (g_arrowcnt3)
			{

				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[8]);

				// １枚のポリゴンの頂点とテクスチャ座標を設定

				SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X + 17, SCREEN_CENTER_Y + 230, 696, 564, 1.0f, 0.0f, 1.0f, 1.0f,
					XMFLOAT4(1.0f, 0.5f, 0.5f, 0.7f));
				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);
				g_arrowcnt3--;
				g_arrowcnt3--;
				if (!g_arrowcnt3)

				{

					*GetExit() = TRUE;
				}
			}
		}
		if (!*GetOptionFlag())
		{
			DrawEnter();
			*GetCursorOption() = 0;
		}

		break;
	default:
		g_phase = MENU;
		break;
	}

}

void IdleText(void)
{
	DIRECTWRITE* DW = GetDirectWrite();
	WCHAR wcText1[] = L"　PRESS ENTER TO CONTINUE";

	DW[0].RT->BeginDraw();
	DW[0].RT->DrawText(wcText1, ARRAYSIZE(wcText1) - 1, DW[0].TextFormat, D2D1::RectF(SCREEN_CENTER_X - 158, TEXTURE_HEIGHT - 50, SCREEN_CENTER_X + 400, TEXTURE_HEIGHT - 25), DW[0].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
	DW[0].RT->EndDraw();
}

void LogoText1(void)
{
	DIRECTWRITE* DW = GetDirectWrite();
	WCHAR wcText1[] = L"　TM(偽)";

	DW[0].RT->BeginDraw();
	DW[0].RT->DrawText(wcText1, ARRAYSIZE(wcText1) - 1, DW[0].TextFormat, D2D1::RectF(SCREEN_CENTER_X + 380, TEXTURE_HEIGHT - 320, SCREEN_CENTER_X + 600, TEXTURE_HEIGHT), DW[0].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
	DW[0].RT->EndDraw();
}

void LogoText2(void)
{
	DIRECTWRITE* DW = GetDirectWrite();
	DW[1].DWriteFactory->CreateTextFormat(L"メイリオ", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 30, L"", &DW[1].TextFormat);
	WCHAR wcText1[] = L"　CUB~~AN ISOMETRIC PUZZLE GAME";

	DW[1].RT->BeginDraw();
	DW[1].RT->DrawText(wcText1, ARRAYSIZE(wcText1) - 1, DW[1].TextFormat, D2D1::RectF(SCREEN_CENTER_X - 293, TEXTURE_HEIGHT - 170, SCREEN_CENTER_X + 600, TEXTURE_HEIGHT - 25), DW[1].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
	DW[1].RT->EndDraw();
}

void LogoText3(void)
{
	DIRECTWRITE* DW = GetDirectWrite();
	WCHAR wcText1[] = L"© 2023 洛陽酒池肉林株式会社(仮)";

	DW[0].RT->BeginDraw();
	DW[0].RT->DrawText(wcText1, ARRAYSIZE(wcText1) - 1, DW[0].TextFormat, D2D1::RectF(SCREEN_CENTER_X - 156, TEXTURE_HEIGHT - 50, SCREEN_CENTER_X + 400, TEXTURE_HEIGHT - 25), DW[0].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
	DW[0].RT->EndDraw();
}

void MenuText1(void)
{
	DIRECTWRITE* DW = GetDirectWrite();

	if (g_arrowcnt2)
		g_arrowcnt2--;
	WCHAR wcText1[] = L"←　  ";
	WCHAR wcText2[] = L"  　→";
	DW[1].RT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Orange), &DW[1].SolidBrush);
	DW[1].DWriteFactory->CreateTextFormat(L"メイリオ", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20, L"ja-jp", &DW[1].TextFormat);

	if (g_arrowcnt1)
	{
		g_arrowcnt1--;
		DW[1].RT->BeginDraw();
		DW[1].RT->DrawText(wcText1, ARRAYSIZE(wcText1) - 1, DW[1].TextFormat, D2D1::RectF(SCREEN_CENTER_X - 20, TEXTURE_HEIGHT - 50, SCREEN_CENTER_X + 400, TEXTURE_HEIGHT - 25), DW[1].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		DW[1].RT->EndDraw();
	}
	else
	{
		DW[0].RT->BeginDraw();
		DW[0].RT->DrawText(wcText1, ARRAYSIZE(wcText1) - 1, DW[0].TextFormat, D2D1::RectF(SCREEN_CENTER_X - 20, TEXTURE_HEIGHT - 50, SCREEN_CENTER_X + 400, TEXTURE_HEIGHT - 25), DW[0].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		DW[0].RT->EndDraw();
	}

	if (g_arrowcnt2)
	{
		g_arrowcnt2--;
		DW[1].RT->BeginDraw();
		DW[1].RT->DrawText(wcText2, ARRAYSIZE(wcText1) - 1, DW[1].TextFormat, D2D1::RectF(SCREEN_CENTER_X - 20, TEXTURE_HEIGHT - 50, SCREEN_CENTER_X + 400, TEXTURE_HEIGHT - 25), DW[1].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		DW[1].RT->EndDraw();
	}
	else
	{
		DW[0].RT->BeginDraw();
		DW[0].RT->DrawText(wcText2, ARRAYSIZE(wcText1) - 1, DW[0].TextFormat, D2D1::RectF(SCREEN_CENTER_X - 20, TEXTURE_HEIGHT - 50, SCREEN_CENTER_X + 400, TEXTURE_HEIGHT - 25), DW[0].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		DW[0].RT->EndDraw();
	}
}

//void DrawOption(void)
//{
//
//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[10]);
//
//	// １枚のポリゴンの頂点とテクスチャ座標を設定
//
//	SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X, SCREEN_CENTER_Y, 720, 380, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 0.8f));
//	// ポリゴン描画
//	GetDeviceContext()->Draw(4, 0);
//
//	DIRECTWRITE* DW = GetDirectWrite();
//	DW[1].RT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::GhostWhite), &DW[1].SolidBrush);
//	DW[1].DWriteFactory->CreateTextFormat(L"メイリオ", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 30, L"ja-jp", &DW[1].TextFormat);
//	WCHAR wcText1[] = L"マスター音量:  ";
//	WCHAR wcText2[] = L"BGM音量:  ";
//	WCHAR wcText3[] = L"SE音量:  ";
//	DW[1].RT->BeginDraw();
//	DW[1].RT->DrawText(wcText1, ARRAYSIZE(wcText1) - 1, DW[1].TextFormat, D2D1::RectF(SCREEN_CENTER_X - 290, TEXTURE_HEIGHT - 480, SCREEN_CENTER_X + 400, TEXTURE_HEIGHT - 25), DW[1].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
//	DW[1].RT->DrawText(wcText2, ARRAYSIZE(wcText2) - 1, DW[1].TextFormat, D2D1::RectF(SCREEN_CENTER_X - 290, TEXTURE_HEIGHT - 430, SCREEN_CENTER_X + 400, TEXTURE_HEIGHT - 25), DW[1].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
//	DW[1].RT->DrawText(wcText3, ARRAYSIZE(wcText3) - 1, DW[1].TextFormat, D2D1::RectF(SCREEN_CENTER_X - 290, TEXTURE_HEIGHT - 380, SCREEN_CENTER_X + 400, TEXTURE_HEIGHT - 25), DW[1].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
//	DW[1].RT->EndDraw();
//}

//void DrawVolumeMas(void)
//{
//
//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[12]);
//
//	// １枚のポリゴンの頂点とテクスチャ座標を設定
//
//	SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X + 100, SCREEN_CENTER_Y - 100, 333, 30, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
//	// ポリゴン描画
//	GetDeviceContext()->Draw(4, 0);
//
//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[11]);
//
//	// １枚のポリゴンの頂点とテクスチャ座標を設定
//
//	SetSpriteLTColor(g_VertexBuffer, 570.0f, 245, 333 * (GetVolume()->master), 30, 0.0f, 0.0f, 1.0f * (GetVolume()->master), 1.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
//	// ポリゴン描画
//	GetDeviceContext()->Draw(4, 0);
//
//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[13]);
//	SetSprite(g_VertexBuffer, SCREEN_CENTER_X + 100 + 333 * 0.5f - 333 * (1.0f - GetVolume()->master), SCREEN_CENTER_Y - 100, 40, 40, 0.0f, 0.0f, 1.0f, 1.0f);
//	GetDeviceContext()->Draw(4, 0);
//}


//void DrawVolumeBGM(void)
//{
//
//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[12]);
//
//	// １枚のポリゴンの頂点とテクスチャ座標を設定
//
//	SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X + 100, SCREEN_CENTER_Y - 50, 333, 30, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
//	// ポリゴン描画
//	GetDeviceContext()->Draw(4, 0);
//
//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[11]);
//
//	// １枚のポリゴンの頂点とテクスチャ座標を設定
//
//	SetSpriteLTColor(g_VertexBuffer, 570.0f, 295, 333 * (GetVolume()->BGM), 30, 0.0f, 0.0f, 1.0f * (GetVolume()->BGM), 1.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
//	// ポリゴン描画
//	GetDeviceContext()->Draw(4, 0);
//
//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[13]);
//	SetSprite(g_VertexBuffer, SCREEN_CENTER_X + 100 + 333 * 0.5f - 333 * (1.0f - GetVolume()->BGM), SCREEN_CENTER_Y - 50, 40, 40, 0.0f, 0.0f, 1.0f, 1.0f);
//	GetDeviceContext()->Draw(4, 0);
//}
//
//void DrawVolumeSE(void)
//{
//
//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[12]);
//
//	// １枚のポリゴンの頂点とテクスチャ座標を設定
//
//	SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X + 100, SCREEN_CENTER_Y, 333, 30, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
//	// ポリゴン描画
//	GetDeviceContext()->Draw(4, 0);
//
//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[11]);
//
//	// １枚のポリゴンの頂点とテクスチャ座標を設定
//
//	SetSpriteLTColor(g_VertexBuffer, 570.0f, 345, 333 * (GetVolume()->SE), 30, 0.0f, 0.0f, 1.0f * (GetVolume()->SE), 1.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
//	// ポリゴン描画
//	GetDeviceContext()->Draw(4, 0);
//
//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[13]);
//	SetSprite(g_VertexBuffer, SCREEN_CENTER_X + 100 + 333 * 0.5f - 333 * (1.0f - GetVolume()->SE), SCREEN_CENTER_Y, 40, 40, 0.0f, 0.0f, 1.0f, 1.0f);
//	GetDeviceContext()->Draw(4, 0);
//}

int* Get_g_Cursor(void)
{
	return &g_cursor;
}

//void DrawOptionSelect(void)
//{
//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[14]);
//	SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X - 200, SCREEN_CENTER_Y - 100 + 50.0f * g_option_cursor, 200, 40, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 0.6f));
//	GetDeviceContext()->Draw(4, 0);
//}

void DrawEnter(void)
{


	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[15]);
	if (g_arrowcnt3)
	{
		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X + 230, SCREEN_CENTER_Y + 250, 64, 32, 0.75f, 0.125f * 5, 0.25f, 0.125f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else
	{
		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X + 230, SCREEN_CENTER_Y + 250, 64, 32, 0.75f, 0.125f, 0.25f, 0.125f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	GetDeviceContext()->Draw(4, 0);
}

//void DrawOptionKey(void)
//{
//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[15]);
//
//
//
//	if (g_arrowcnt1)
//	{
//		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X - 310, SCREEN_CENTER_Y - 135, 64, 32, 0.25f, 0.5f, 0.25f, 0.125f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
//		GetDeviceContext()->Draw(4, 0);
//
//		g_arrowcnt1--;
//		if (!g_arrowcnt1)
//			g_option_flag = FALSE;
//	}
//	else
//	{
//		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X - 310, SCREEN_CENTER_Y - 135, 64, 32, 0.25f, 0.0f, 0.25f, 0.125f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
//		GetDeviceContext()->Draw(4, 0);
//	}
//	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[16]);
//	if (g_arrowcnt3)
//	{
//		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X - 230, SCREEN_CENTER_Y + 50, 32, 32, 0.0f, 0.5f, 0.125f, 0.07143f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
//		GetDeviceContext()->Draw(4, 0);
//		g_arrowcnt3--;
//	}
//	else {
//		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X - 230, SCREEN_CENTER_Y + 50, 32, 32, 0.0f, 0.0f, 0.125f, 0.07143f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
//		GetDeviceContext()->Draw(4, 0);
//	}
//	if (g_arrowcnt2)
//	{
//		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X - 230, SCREEN_CENTER_Y + 75, 32, 32, 0.125f, 0.5f, 0.125f, 0.07143f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
//		GetDeviceContext()->Draw(4, 0);
//		g_arrowcnt2--;
//	}
//	else
//	{
//		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X - 230, SCREEN_CENTER_Y + 75, 32, 32, 0.125f, 0.0f, 0.125f, 0.07143f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
//		GetDeviceContext()->Draw(4, 0);
//	}
//	if (g_arrowcnt5)
//	{
//		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X + 115, SCREEN_CENTER_Y + 62.5, 32, 32, 0.375f, 0.5f, 0.125f, 0.07143f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
//		GetDeviceContext()->Draw(4, 0);
//		g_arrowcnt5--;
//	}
//	else
//	{
//		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X + 115, SCREEN_CENTER_Y + 62.5, 32, 32, 0.375f, 0.0f, 0.125f, 0.07143f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
//		GetDeviceContext()->Draw(4, 0);
//	}
//	if (g_arrowcnt4)
//	{
//		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X +85, SCREEN_CENTER_Y + 62.5, 32, 32, 0.25f, 0.5f, 0.125f, 0.07143f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
//		GetDeviceContext()->Draw(4, 0);
//		g_arrowcnt4--;
//	}
//	else
//	{
//		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X + 85, SCREEN_CENTER_Y + 62.5, 32, 32, 0.25f, 0.0f, 0.125f, 0.07143f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
//		GetDeviceContext()->Draw(4, 0);
//	}
//}

//void UpdateOption(void)
//{
//	{
//
//		if (GetKeyboardTrigger(DIK_ESCAPE))
//		{
//
//			g_option_cursor = 0;
//			PlaySound(SOUND_LABEL_SE_menu1);
//			g_arrowcnt1 = 20;
//		}
//		else if (GetKeyboardTrigger(DIK_DOWN))
//		{
//
//			g_arrowcnt2 = 20;
//			g_option_cursor++;
//			g_option_cursor %= 3;
//			PlaySound(SOUND_LABEL_SE_menu1);
//		}
//		else if (GetKeyboardTrigger(DIK_UP))
//		{
//
//			g_arrowcnt3 = 20;
//			g_option_cursor--;
//			if (g_option_cursor < 0)
//				g_option_cursor += 3;
//			PlaySound(SOUND_LABEL_SE_menu1);
//		}
//		else if (GetKeyboardRepeat(DIK_LEFT))
//		{
//			g_arrowcnt4 = 20;
//
//			PlaySound(SOUND_LABEL_SE_rim_scan);
//			switch (g_option_cursor)
//			{
//			case 0:
//				GetVolume()->master -= 0.01f;
//				break;
//			case 1:
//				GetVolume()->BGM -= 0.01f;
//				break;
//			case 2:
//				GetVolume()->SE -= 0.01f;
//				break;
//			}
//
//		}
//		else if (GetKeyboardRepeat(DIK_RIGHT))
//		{
//			g_arrowcnt5 = 20;
//
//			PlaySound(SOUND_LABEL_SE_rim_scan);
//			switch (g_option_cursor)
//			{
//			case 0:
//				GetVolume()->master += 0.01f;
//				break;
//			case 1:
//				GetVolume()->BGM += 0.01f;
//				break;
//			case 2:
//				GetVolume()->SE += 0.01f;
//				break;
//			}
//		}
//
//		if (GetVolume()->master > 1.0f)
//			GetVolume()->master = 1.0f;
//		if (GetVolume()->BGM > 1.0f)
//			GetVolume()->BGM = 1.0f;
//		if (GetVolume()->SE > 1.0f)
//			GetVolume()->SE = 1.0f;
//		if (GetVolume()->master < 0.0f)
//			GetVolume()->master = 0.0f;
//		if (GetVolume()->BGM < 0.0f)
//			GetVolume()->BGM = 0.0f;
//		if (GetVolume()->SE < 0.0f)
//			GetVolume()->SE = 0.0f;
//		SetVolume();
//	}
//
//}

void SetPhase(void)
{
	g_phase = MENU;
}
BOOL* GetGReturn(void)
{
	return &g_return;
}
BOOL* GetReturn2(void)
{
	return &g_return2;
}