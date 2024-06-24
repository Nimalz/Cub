#include "showInput.h"
#include "main.h"
#include "input.h"
#include "renderer.h"
#include "sprite.h"
#include "text.h"
#include "field.h"
#include "bullet.h"
#include "text.h"
#include "option.h"

#define TEXTURE_MAX					(2)		// テクスチャの数

#define BUTTON_ANI_FRAME			(10)
#define BUTTON_SIZE_WID				(64*1.3f)
#define BUTTON_SIZE_HET				(32*1.3f)
#define ESC_LT_X					(42)
#define ESC_LT_Y					(95)
#define ENTER_LT_X					(ESC_LT_X+150+45)
#define ENTER_LT_Y					(ESC_LT_Y-50)
#define ARROW_LT_X					(ENTER_LT_X)
#define ARROW_LT_Y					(ENTER_LT_Y+50)
#define ARROW_INTERVAL				(25*1.3f)
#define E_LT_X						(ARROW_LT_X)
#define E_LT_Y						(ARROW_LT_Y+100)
#define Q_LT_X						(E_LT_X-50)
#define Q_LT_Y						(E_LT_Y)
#define R_LT_X						(E_LT_X+50)
#define R_LT_Y						(E_LT_Y)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char* g_TexturName[] = {
	"data/TEXTURE/Keyboard Extras.png",
	"data/TEXTURE/Keyboard Letters and Symbols.png",
};

static BOOL			g_Load = FALSE;			 // 初期化を行ったかのフラグ
static InputDetect  g_InputState;			 // 輸入ステイタスを記録
static InputCounter g_InputCounter;			 // ボタンアニメイシ用
static BOOL         g_ShowSI = TRUE;				 // 操作指示を表示するか？

void InitSI(void)
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

	g_InputState.T_ESC = FALSE;
	g_InputState.T_ENTER = FALSE;
	g_InputState.T_UP = FALSE;
	g_InputState.T_DOWN = FALSE;
	g_InputState.T_LEFT = FALSE;
	g_InputState.T_RIGHT = FALSE;
	g_InputState.T_ABI1 = FALSE;
	g_InputState.T_ABI2 = FALSE;
	g_InputState.T_RESET = FALSE;

	g_InputCounter.C_ESC = 0;
	g_InputCounter.C_ENTER = 0;
	g_InputCounter.C_UP = 0;
	g_InputCounter.C_DOWN = 0;
	g_InputCounter.C_LEFT = 0;
	g_InputCounter.C_RIGHT = 0;
	g_InputCounter.C_ABI1 = 0;
	g_InputCounter.C_ABI2 = 0;
	g_InputCounter.C_RESET = 0;

	//g_ShowSI = TRUE;
	g_Load = TRUE;
}

void UninitSI(void)
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

void UpdateSI(void)
{
	if (!*GetOptionFlag())
	{
		if (GetKeyboardTrigger(DIK_ESCAPE))
			g_InputState.T_ESC = TRUE;
		if (GetKeyboardTrigger(DIK_RETURN))
			g_InputState.T_ENTER = TRUE;

		if (GetKeyboardTrigger(DIK_UP) || GetKeyboardPress(DIK_UP))
			g_InputState.T_UP = TRUE;
		else if (GetKeyboardTrigger(DIK_DOWN) || GetKeyboardPress(DIK_DOWN))
			g_InputState.T_DOWN = TRUE;

		if (GetKeyboardTrigger(DIK_LEFT) || GetKeyboardPress(DIK_LEFT))
			g_InputState.T_LEFT = TRUE;
		else if (GetKeyboardTrigger(DIK_RIGHT) || GetKeyboardPress(DIK_RIGHT))
			g_InputState.T_RIGHT = TRUE;

		if (GetKeyboardTrigger(DIK_E))
			g_InputState.T_ABI1 = TRUE;
		if (GetKeyboardTrigger(DIK_Q))
			g_InputState.T_ABI2 = TRUE;
		if (GetKeyboardTrigger(DIK_R))
			g_InputState.T_RESET = TRUE;

		if (g_InputState.T_ESC)
			g_InputCounter.C_ESC = BUTTON_ANI_FRAME;
		if (g_InputState.T_ENTER)
			g_InputCounter.C_ENTER = BUTTON_ANI_FRAME;
		if (g_InputState.T_UP)
			g_InputCounter.C_UP = BUTTON_ANI_FRAME;
		if (g_InputState.T_DOWN)
			g_InputCounter.C_DOWN = BUTTON_ANI_FRAME;
		if (g_InputState.T_LEFT)
			g_InputCounter.C_LEFT = BUTTON_ANI_FRAME;
		if (g_InputState.T_RIGHT)
			g_InputCounter.C_RIGHT = BUTTON_ANI_FRAME;
		if (g_InputState.T_ABI1)
			g_InputCounter.C_ABI1 = BUTTON_ANI_FRAME;
		if (g_InputState.T_ABI2)
			g_InputCounter.C_ABI2 = BUTTON_ANI_FRAME;
		if (g_InputState.T_RESET)
			g_InputCounter.C_RESET = BUTTON_ANI_FRAME;

		g_InputState.T_ABI1 = FALSE;
		g_InputState.T_ABI2 = FALSE;
		g_InputState.T_DOWN = FALSE;
		g_InputState.T_ENTER = FALSE;
		g_InputState.T_ESC = FALSE;
		g_InputState.T_LEFT = FALSE;
		g_InputState.T_RESET = FALSE;
		g_InputState.T_RIGHT = FALSE;
		g_InputState.T_UP = FALSE;
	}


}

void DrawSI(void)
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

	if (g_ShowSI)
	{
		XMFLOAT4 diff(1.0f, 1.0f, 1.0f, 1.0f);
		XMFLOAT4 diffe(1.0f, 1.0f, 1.0f, 1.0f);
		XMFLOAT4 diffesc(1.0f, 1.0f, 1.0f, 1.0f);
		XMFLOAT4 diffenter(1.0f, 1.0f, 1.0f, 1.0f);
		if (GetMenuFlag())
		{
			diff.y = 0.2f;
			diff.z = 0.2f;
			diffe.y = 0.2f;
			diffe.z = 0.2f;
		}
		else if (!GetBullet()->use || (GetBullet()->use && (GetSelect() && (!GetBlockInput()))))
		{
			diffenter.y = 0.2f;
			diffenter.z = 0.2f;
		}

		if (GetBullet()->use && (!GetSelect()))
		{

			diffesc.y = 0.2f;
			diffesc.z = 0.2f;
		}

		if (GetBullet()->use && (GetSelect()))
		{
			diffe.y = 0.2f;
			diffe.z = 0.2f;
		}
		{
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

			if (g_InputCounter.C_ESC)
			{
				g_InputCounter.C_ESC--;
				SetSpriteColor(g_VertexBuffer, ESC_LT_X, ESC_LT_Y, BUTTON_SIZE_WID, BUTTON_SIZE_HET, 0.25f, 0.5f, 0.25f, 0.125f, diffesc);
				GetDeviceContext()->Draw(4, 0);
			}
			else
			{
				SetSpriteColor(g_VertexBuffer, ESC_LT_X, ESC_LT_Y, BUTTON_SIZE_WID, BUTTON_SIZE_HET, 0.25f, 0.0f, 0.25f, 0.125f, diffesc);
				GetDeviceContext()->Draw(4, 0);
			}
			if (g_InputCounter.C_ENTER)
			{
				g_InputCounter.C_ENTER--;
				SetSpriteColor(g_VertexBuffer, ENTER_LT_X, ENTER_LT_Y, BUTTON_SIZE_WID, BUTTON_SIZE_HET, 0.75f, 0.625f, 0.25f, 0.125f, diffenter);
				GetDeviceContext()->Draw(4, 0);
			}
			else
			{
				SetSpriteColor(g_VertexBuffer, ENTER_LT_X, ENTER_LT_Y, BUTTON_SIZE_WID, BUTTON_SIZE_HET, 0.75f, 0.125f, 0.25f, 0.125f, diffenter);
				GetDeviceContext()->Draw(4, 0);
			}

			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

			if (g_InputCounter.C_UP)
			{
				g_InputCounter.C_UP--;
				SetSpriteColor(g_VertexBuffer, ARROW_LT_X, ARROW_LT_Y, BUTTON_SIZE_HET, BUTTON_SIZE_HET, 0.0f, 0.5f, 0.125f, 0.07143f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
				GetDeviceContext()->Draw(4, 0);
			}
			else {
				SetSpriteColor(g_VertexBuffer, ARROW_LT_X, ARROW_LT_Y, BUTTON_SIZE_HET, BUTTON_SIZE_HET, 0.0f, 0.0f, 0.125f, 0.07143f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
				GetDeviceContext()->Draw(4, 0);
			}
			if (g_InputCounter.C_DOWN)
			{
				g_InputCounter.C_DOWN--;
				SetSpriteColor(g_VertexBuffer, ARROW_LT_X, ARROW_LT_Y + ARROW_INTERVAL, BUTTON_SIZE_HET, BUTTON_SIZE_HET, 0.125f, 0.5f, 0.125f, 0.07143f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
				GetDeviceContext()->Draw(4, 0);
			}
			else
			{
				SetSpriteColor(g_VertexBuffer, ARROW_LT_X, ARROW_LT_Y + ARROW_INTERVAL, BUTTON_SIZE_HET, BUTTON_SIZE_HET, 0.125f, 0.0f, 0.125f, 0.07143f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
				GetDeviceContext()->Draw(4, 0);
			}
			if (g_InputCounter.C_LEFT)
			{
				g_InputCounter.C_LEFT--;
				SetSpriteColor(g_VertexBuffer, ARROW_LT_X - ARROW_INTERVAL, ARROW_LT_Y + ARROW_INTERVAL, BUTTON_SIZE_HET, BUTTON_SIZE_HET, 0.25f, 0.5f, 0.125f, 0.07143f, diff);
				GetDeviceContext()->Draw(4, 0);
			}
			else
			{
				SetSpriteColor(g_VertexBuffer, ARROW_LT_X - ARROW_INTERVAL, ARROW_LT_Y + ARROW_INTERVAL, BUTTON_SIZE_HET, BUTTON_SIZE_HET, 0.25f, 0.0f, 0.125f, 0.07143f, diff);
				GetDeviceContext()->Draw(4, 0);
			}
			if (g_InputCounter.C_RIGHT)
			{
				g_InputCounter.C_RIGHT--;
				SetSpriteColor(g_VertexBuffer, ARROW_LT_X + ARROW_INTERVAL, ARROW_LT_Y + ARROW_INTERVAL, BUTTON_SIZE_HET, BUTTON_SIZE_HET, 0.375f, 0.5f, 0.125f, 0.07143f, diff);
				GetDeviceContext()->Draw(4, 0);
			}
			else
			{
				SetSpriteColor(g_VertexBuffer, ARROW_LT_X + ARROW_INTERVAL, ARROW_LT_Y + ARROW_INTERVAL, BUTTON_SIZE_HET, BUTTON_SIZE_HET, 0.375f, 0.0f, 0.125f, 0.07143f, diff);
				GetDeviceContext()->Draw(4, 0);
			}
			if (g_InputCounter.C_ABI1)
			{
				g_InputCounter.C_ABI1--;
				SetSpriteColor(g_VertexBuffer, E_LT_X, E_LT_Y, BUTTON_SIZE_HET, BUTTON_SIZE_HET, 0.5f, 0.64286f, 0.125f, 0.07143f, diffe);
				GetDeviceContext()->Draw(4, 0);
			}
			else
			{
				SetSpriteColor(g_VertexBuffer, E_LT_X, E_LT_Y, BUTTON_SIZE_HET, BUTTON_SIZE_HET, 0.5f, 0.14286f, 0.125f, 0.07143f, diffe);
				GetDeviceContext()->Draw(4, 0);
			}
			if (g_InputCounter.C_ABI2)
			{
				g_InputCounter.C_ABI2--;
				SetSpriteColor(g_VertexBuffer, Q_LT_X, Q_LT_Y, BUTTON_SIZE_HET, BUTTON_SIZE_HET, 0.0f, 0.14286f * 2.0f + 0.5f, 0.125f, 0.07143f, diff);
				GetDeviceContext()->Draw(4, 0);
			}
			else
			{
				SetSpriteColor(g_VertexBuffer, Q_LT_X, Q_LT_Y, BUTTON_SIZE_HET, BUTTON_SIZE_HET, 0.0f, 0.14286f * 2.0f, 0.125f, 0.07143f, diff);
				GetDeviceContext()->Draw(4, 0);
			}

			if (g_InputCounter.C_RESET)
			{
				g_InputCounter.C_RESET--;
				SetSpriteColor(g_VertexBuffer, R_LT_X, R_LT_Y, BUTTON_SIZE_HET, BUTTON_SIZE_HET, 0.125f, 0.14286f * 2.0f + 0.5f, 0.125f, 0.07143f, diff);
				GetDeviceContext()->Draw(4, 0);
			}
			else
			{
				SetSpriteColor(g_VertexBuffer, R_LT_X, R_LT_Y, BUTTON_SIZE_HET, BUTTON_SIZE_HET, 0.125f, 0.14286f * 2.0f, 0.125f, 0.07143f, diff);
				GetDeviceContext()->Draw(4, 0);
			}
		}
			DrawSIText();
	}
	
}

InputDetect* GetInputStates(void)
{
	return &g_InputState;
}

void DrawSIText(void)
{
	DIRECTWRITE* DW = GetDirectWrite();
	DW[1].RT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::GhostWhite), &DW[1].SolidBrush);
	DW[1].DWriteFactory->CreateTextFormat(L"メイリオ", nullptr, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20, L"ja-jp", &DW[1].TextFormat);
	DW[0].RT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::GhostWhite), &DW[0].SolidBrush);
	DW[0].DWriteFactory->CreateTextFormat(L"メイリオ", nullptr, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 16, L"ja-jp", &DW[0].TextFormat);
	WCHAR wcText1[] = L"移動";
	WCHAR wcText2[] = L"輪郭表示";
	WCHAR wcText3[] = L"リセット";
	WCHAR wcText4[] = L"地形移動モードへ";
	WCHAR wcText5[] = L"確認";
	WCHAR wcText6[] = L"選択";
	WCHAR wcText7[] = L"通常モードへ";
	WCHAR wcText8[] = L"方向入力";
	WCHAR wcText9[] = L"取り消す";
	WCHAR wcText10[] = L"メニュー";
	WCHAR wcText11[] = L"戻る";
	WCHAR wcText12[] = L"タイトルへ";
	WCHAR wcText13[] = L"オプション";
	WCHAR wcText14[] = L"ゲーム終了";

	if (!GetMenuFlag() && !GetBullet()->use)
	{

		DW[1].RT->BeginDraw();
		DW[1].RT->DrawText(wcText1, ARRAYSIZE(wcText1) - 1, DW[1].TextFormat, D2D1::RectF(ARROW_LT_X + 60, ARROW_LT_Y, ARROW_LT_X + 300, ARROW_LT_Y + 100), DW[1].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		DW[1].RT->EndDraw();
		DW[0].RT->BeginDraw();
		DW[0].RT->DrawText(wcText2, ARRAYSIZE(wcText2) - 1, DW[0].TextFormat, D2D1::RectF(Q_LT_X - 33, Q_LT_Y + 20, Q_LT_X + 300, Q_LT_Y + 300), DW[0].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		DW[0].RT->DrawText(wcText3, ARRAYSIZE(wcText3) - 1, DW[0].TextFormat, D2D1::RectF(R_LT_X - 33, R_LT_Y + 20, R_LT_X + 300, R_LT_Y + 300), DW[0].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		DW[0].RT->DrawText(wcText4, ARRAYSIZE(wcText4) - 1, DW[0].TextFormat, D2D1::RectF(E_LT_X - 62, E_LT_Y - 42, E_LT_X + 300, E_LT_Y + 300), DW[0].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		DW[0].RT->DrawText(wcText10, ARRAYSIZE(wcText10) - 1, DW[0].TextFormat, D2D1::RectF(ESC_LT_X - 32, ESC_LT_Y + 20, ESC_LT_X + 300, ESC_LT_Y + 300), DW[0].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);

		DW[0].RT->EndDraw();
	}
	else if (GetMenuFlag())
	{
		DW[1].RT->BeginDraw();
		DW[1].RT->DrawText(wcText1, ARRAYSIZE(wcText1) - 1, DW[1].TextFormat, D2D1::RectF(ARROW_LT_X + 60, ARROW_LT_Y, ARROW_LT_X + 300, ARROW_LT_Y + 100), DW[1].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		DW[1].RT->DrawText(wcText5, ARRAYSIZE(wcText5) - 1, DW[1].TextFormat, D2D1::RectF(ENTER_LT_X + 60, ENTER_LT_Y - 15, ENTER_LT_X + 300, ENTER_LT_Y + 100), DW[1].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		DW[1].RT->EndDraw();

		DW[0].RT->BeginDraw();
		DW[0].RT->DrawText(wcText11, ARRAYSIZE(wcText11) - 1, DW[0].TextFormat, D2D1::RectF(ESC_LT_X - 15, ESC_LT_Y + 20, ESC_LT_X + 300, ESC_LT_Y + 300), DW[0].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		DW[0].RT->EndDraw();

		switch (GetMenuCursor())
		{
			//タイトル

		case 0:
			DW[0].RT->BeginDraw();
			DW[0].RT->DrawText(wcText12, ARRAYSIZE(wcText12) - 1, DW[0].TextFormat, D2D1::RectF(ESC_LT_X + 110, ESC_LT_Y - 50, ESC_LT_X + 120, ESC_LT_Y + 300), DW[0].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
			DW[0].RT->EndDraw();
			break;
			//オプション
		case 1:
			DW[0].RT->BeginDraw();
			DW[0].RT->DrawText(wcText13, ARRAYSIZE(wcText13) - 1, DW[0].TextFormat, D2D1::RectF(ESC_LT_X + 110, ESC_LT_Y - 50, ESC_LT_X + 120, ESC_LT_Y + 300), DW[0].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
			DW[0].RT->EndDraw();
			break;
			break;
			//終了
		case 2:
			DW[0].RT->BeginDraw();
			DW[0].RT->DrawText(wcText14, ARRAYSIZE(wcText14) - 1, DW[0].TextFormat, D2D1::RectF(ESC_LT_X + 110, ESC_LT_Y - 50, ESC_LT_X + 120, ESC_LT_Y + 300), DW[0].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
			DW[0].RT->EndDraw();
			break;
			break;

		}
	}
	else if (GetBullet()->use && !GetSelect())
	{
		DW[1].RT->BeginDraw();
		DW[1].RT->DrawText(wcText1, ARRAYSIZE(wcText1) - 1, DW[1].TextFormat, D2D1::RectF(ARROW_LT_X + 60, ARROW_LT_Y, ARROW_LT_X + 300, ARROW_LT_Y + 100), DW[1].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		DW[1].RT->DrawText(wcText6, ARRAYSIZE(wcText6) - 1, DW[1].TextFormat, D2D1::RectF(ENTER_LT_X + 60, ENTER_LT_Y - 15, ENTER_LT_X + 300, ENTER_LT_Y + 100), DW[1].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		DW[1].RT->EndDraw();

		DW[0].RT->BeginDraw();
		DW[0].RT->DrawText(wcText2, ARRAYSIZE(wcText2) - 1, DW[0].TextFormat, D2D1::RectF(Q_LT_X - 33, Q_LT_Y + 20, Q_LT_X + 300, Q_LT_Y + 300), DW[0].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		DW[0].RT->DrawText(wcText3, ARRAYSIZE(wcText3) - 1, DW[0].TextFormat, D2D1::RectF(R_LT_X - 33, R_LT_Y + 20, R_LT_X + 300, R_LT_Y + 300), DW[0].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		DW[0].RT->DrawText(wcText7, ARRAYSIZE(wcText7) - 1, DW[0].TextFormat, D2D1::RectF(E_LT_X - 48, E_LT_Y - 42, E_LT_X + 300, E_LT_Y + 300), DW[0].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		DW[0].RT->EndDraw();

	}
	else if (GetBullet()->use && (GetSelect() && (!GetBlockInput())))
	{
		DW[1].RT->BeginDraw();
		DW[1].RT->DrawText(wcText8, ARRAYSIZE(wcText8) - 1, DW[1].TextFormat, D2D1::RectF(ARROW_LT_X + 60, ARROW_LT_Y, ARROW_LT_X + 300, ARROW_LT_Y + 100), DW[1].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		DW[1].RT->DrawText(wcText9, ARRAYSIZE(wcText9) - 1, DW[1].TextFormat, D2D1::RectF(ESC_LT_X - 35, ESC_LT_Y + 20, ESC_LT_X + 300, ESC_LT_Y + 100), DW[1].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);

		DW[1].RT->EndDraw();

		DW[0].RT->BeginDraw();
		DW[0].RT->DrawText(wcText2, ARRAYSIZE(wcText2) - 1, DW[0].TextFormat, D2D1::RectF(Q_LT_X - 33, Q_LT_Y + 20, Q_LT_X + 300, Q_LT_Y + 300), DW[0].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		DW[0].RT->DrawText(wcText3, ARRAYSIZE(wcText3) - 1, DW[0].TextFormat, D2D1::RectF(R_LT_X - 33, R_LT_Y + 20, R_LT_X + 300, R_LT_Y + 300), DW[0].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		DW[0].RT->EndDraw();
	}
	else
	{
		DW[1].RT->BeginDraw();
		DW[1].RT->DrawText(wcText5, ARRAYSIZE(wcText5) - 1, DW[1].TextFormat, D2D1::RectF(ENTER_LT_X + 60, ENTER_LT_Y - 15, ENTER_LT_X + 300, ENTER_LT_Y + 100), DW[1].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		DW[1].RT->DrawText(wcText8, ARRAYSIZE(wcText8) - 1, DW[1].TextFormat, D2D1::RectF(ARROW_LT_X + 60, ARROW_LT_Y, ARROW_LT_X + 300, ARROW_LT_Y + 100), DW[1].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		DW[1].RT->DrawText(wcText9, ARRAYSIZE(wcText9) - 1, DW[1].TextFormat, D2D1::RectF(ESC_LT_X - 35, ESC_LT_Y + 20, ESC_LT_X + 300, ESC_LT_Y + 100), DW[1].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		DW[1].RT->EndDraw();

		DW[0].RT->BeginDraw();
		DW[0].RT->DrawText(wcText2, ARRAYSIZE(wcText2) - 1, DW[0].TextFormat, D2D1::RectF(Q_LT_X - 33, Q_LT_Y + 20, Q_LT_X + 300, Q_LT_Y + 300), DW[0].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		DW[0].RT->DrawText(wcText3, ARRAYSIZE(wcText3) - 1, DW[0].TextFormat, D2D1::RectF(R_LT_X - 33, R_LT_Y + 20, R_LT_X + 300, R_LT_Y + 300), DW[0].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
		DW[0].RT->EndDraw();
	}


}

BOOL* GetShowSI(void)
{
	return &g_ShowSI;
}
