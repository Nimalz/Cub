#include "main.h"
#include "option.h"
#include "input.h"
#include "sound.h"
#include "sprite.h"
#include "text.h"
#include "showInput.h"
#define TEXTURE_MAX					(7)				// テクスチャの数
#define ARROW_Y_OFFSET				(57)
#define OPTION_MAX					(4)
//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int						g_option_cursor = 0;		//Option内部カーソル

static int						g_arrowcnt1 = 0;			//アニメイシ用
static int						g_arrowcnt2 = 0;			//アニメイシ用
static int						g_arrowcnt3 = 0;			//アニメイシ用
static int						g_arrowcnt4 = 0;			//アニメイシ用
static int						g_arrowcnt5 = 0;			//アニメイシ用
static BOOL						g_option_flag = FALSE;		//option　flag

static BOOL						g_Load = FALSE;


static ID3D11Buffer* g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報
static char* g_TexturName[TEXTURE_MAX] = {

	"data/TEXTURE/UI_BG.png",
	"data/TEXTURE/slide1.png",
	"data/TEXTURE/slide2.png",
	"data/TEXTURE/ball.png",
	"data/TEXTURE/white.png",
	"data/TEXTURE/Keyboard Extras.png",
	"data/TEXTURE/Keyboard Letters and Symbols.png",

};

HRESULT InitOption(void)
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

	g_arrowcnt1 = 0;
	g_arrowcnt2 = 0;
	g_arrowcnt3 = 0;
	g_option_flag = FALSE;
	g_option_cursor = 0;
	g_arrowcnt4 = 0;
	g_arrowcnt5 = 0;

	g_Load = TRUE;
	return S_OK;
}

void UninitOption(void)
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

void UpdateOption(void)
{
	{

		if (GetKeyboardTrigger(DIK_ESCAPE))
		{


			PlaySound(SOUND_LABEL_SE_cancel);
			g_arrowcnt1 = 20;
		}
		else if (GetKeyboardTrigger(DIK_DOWN))
		{

			g_arrowcnt2 = 20;
			g_option_cursor++;
			g_option_cursor %= OPTION_MAX;
			PlaySound(SOUND_LABEL_SE_menu1);
		}
		else if (GetKeyboardTrigger(DIK_UP))
		{

			g_arrowcnt3 = 20;
			g_option_cursor--;
			if (g_option_cursor < 0)
				g_option_cursor += OPTION_MAX;
			PlaySound(SOUND_LABEL_SE_menu1);
		}
		else if (GetKeyboardRepeat(DIK_LEFT))
		{
			g_arrowcnt4 = 20;

			PlaySound(SOUND_LABEL_SE_rim_scan);
			switch (g_option_cursor)
			{
			case 0:
				GetVolume()->master -= 0.01f;
				break;
			case 1:
				GetVolume()->BGM -= 0.01f;
				break;
			case 2:
				GetVolume()->SE -= 0.01f;
				break;
			}

		}
		else if (GetKeyboardRepeat(DIK_RIGHT))
		{
			g_arrowcnt5 = 20;

			PlaySound(SOUND_LABEL_SE_rim_scan);
			switch (g_option_cursor)
			{
			case 0:
				GetVolume()->master += 0.01f;
				break;
			case 1:
				GetVolume()->BGM += 0.01f;
				break;
			case 2:
				GetVolume()->SE += 0.01f;
				break;
			}
		}
		else if (GetKeyboardTrigger(DIK_RETURN))
		{
			if (g_option_cursor == 3)
			{
				if (*GetShowSI())
					*GetShowSI() = FALSE;
				else
					*GetShowSI() = TRUE;
			}
		}

		if (GetVolume()->master > 1.0f)
			GetVolume()->master = 1.0f;
		if (GetVolume()->BGM > 1.0f)
			GetVolume()->BGM = 1.0f;
		if (GetVolume()->SE > 1.0f)
			GetVolume()->SE = 1.0f;
		if (GetVolume()->master < 0.0f)
			GetVolume()->master = 0.0f;
		if (GetVolume()->BGM < 0.0f)
			GetVolume()->BGM = 0.0f;
		if (GetVolume()->SE < 0.0f)
			GetVolume()->SE = 0.0f;
		SetVolume();
	}

}

void DrawOptionSelect(void)
{
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[4]);
	SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X - 200, SCREEN_CENTER_Y - 100 + 50.0f * g_option_cursor, 200, 40, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 0.6f));
	GetDeviceContext()->Draw(4, 0);
}

void DrawOptionKey(void)
{
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[5]);



	if (g_arrowcnt1)
	{
		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X - 310, SCREEN_CENTER_Y - 135, 64, 32, 0.25f, 0.5f, 0.25f, 0.125f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);

		g_arrowcnt1--;
		if (!g_arrowcnt1)
		{
			g_option_flag = FALSE;
			g_option_cursor = 0;
		}

	}
	else
	{
		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X - 310, SCREEN_CENTER_Y - 135, 64, 32, 0.25f, 0.0f, 0.25f, 0.125f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);
	}
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[6]);
	if (g_arrowcnt3)
	{
		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X - 230, SCREEN_CENTER_Y + 50 + ARROW_Y_OFFSET, 32, 32, 0.0f, 0.5f, 0.125f, 0.07143f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);
		g_arrowcnt3--;
	}
	else {
		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X - 230, SCREEN_CENTER_Y + 50 + ARROW_Y_OFFSET, 32, 32, 0.0f, 0.0f, 0.125f, 0.07143f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);
	}
	if (g_arrowcnt2)
	{
		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X - 230, SCREEN_CENTER_Y + 75 + ARROW_Y_OFFSET, 32, 32, 0.125f, 0.5f, 0.125f, 0.07143f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);
		g_arrowcnt2--;
	}
	else
	{
		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X - 230, SCREEN_CENTER_Y + 75 + ARROW_Y_OFFSET, 32, 32, 0.125f, 0.0f, 0.125f, 0.07143f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);
	}
	if (g_arrowcnt5)
	{
		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X + 115, SCREEN_CENTER_Y + 62.5 + ARROW_Y_OFFSET, 32, 32, 0.375f, 0.5f, 0.125f, 0.07143f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);
		g_arrowcnt5--;
	}
	else
	{
		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X + 115, SCREEN_CENTER_Y + 62.5 + ARROW_Y_OFFSET, 32, 32, 0.375f, 0.0f, 0.125f, 0.07143f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);
	}
	if (g_arrowcnt4)
	{
		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X + 85, SCREEN_CENTER_Y + 62.5 + ARROW_Y_OFFSET, 32, 32, 0.25f, 0.5f, 0.125f, 0.07143f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);
		g_arrowcnt4--;
	}
	else
	{
		SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X + 85, SCREEN_CENTER_Y + 62.5 + ARROW_Y_OFFSET, 32, 32, 0.25f, 0.0f, 0.125f, 0.07143f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		GetDeviceContext()->Draw(4, 0);
	}
}
void DrawOption(void)
{

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

	// １枚のポリゴンの頂点とテクスチャ座標を設定

	SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X, SCREEN_CENTER_Y, 720, 380, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 0.8f));
	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);

	DIRECTWRITE* DW = GetDirectWrite();
	DW[1].RT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::GhostWhite), &DW[1].SolidBrush);
	DW[1].DWriteFactory->CreateTextFormat(L"メイリオ", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 30, L"ja-jp", &DW[1].TextFormat);
	WCHAR wcText1[] = L"マスター音量:  ";
	WCHAR wcText2[] = L"BGM音量:  ";
	WCHAR wcText3[] = L"SE音量:  ";
	//WCHAR wcText4[] = L":";
	DW[1].RT->BeginDraw();
	DW[1].RT->DrawText(wcText1, ARRAYSIZE(wcText1) - 1, DW[1].TextFormat, D2D1::RectF(SCREEN_CENTER_X - 290, SCREEN_HEIGHT - 480, SCREEN_CENTER_X + 400, SCREEN_HEIGHT - 25), DW[1].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
	DW[1].RT->DrawText(wcText2, ARRAYSIZE(wcText2) - 1, DW[1].TextFormat, D2D1::RectF(SCREEN_CENTER_X - 290, SCREEN_HEIGHT - 430, SCREEN_CENTER_X + 400, SCREEN_HEIGHT - 25), DW[1].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
	DW[1].RT->DrawText(wcText3, ARRAYSIZE(wcText3) - 1, DW[1].TextFormat, D2D1::RectF(SCREEN_CENTER_X - 290, SCREEN_HEIGHT - 380, SCREEN_CENTER_X + 400, SCREEN_HEIGHT - 25), DW[1].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
	DW[1].RT->EndDraw();
}
void DrawVolumeMas(void)
{

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

	// １枚のポリゴンの頂点とテクスチャ座標を設定

	SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X + 100, SCREEN_CENTER_Y - 100, 333, 30, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

	// １枚のポリゴンの頂点とテクスチャ座標を設定

	SetSpriteLTColor(g_VertexBuffer, 570.0f, 245, 333 * (GetVolume()->master), 30, 0.0f, 0.0f, 1.0f * (GetVolume()->master), 1.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);
	SetSprite(g_VertexBuffer, SCREEN_CENTER_X + 100 + 333 * 0.5f - 333 * (1.0f - GetVolume()->master), SCREEN_CENTER_Y - 100, 40, 40, 0.0f, 0.0f, 1.0f, 1.0f);
	GetDeviceContext()->Draw(4, 0);
}
void DrawVolumeBGM(void)
{

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

	// １枚のポリゴンの頂点とテクスチャ座標を設定

	SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X + 100, SCREEN_CENTER_Y - 50, 333, 30, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

	// １枚のポリゴンの頂点とテクスチャ座標を設定

	SetSpriteLTColor(g_VertexBuffer, 570.0f, 295, 333 * (GetVolume()->BGM), 30, 0.0f, 0.0f, 1.0f * (GetVolume()->BGM), 1.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);
	SetSprite(g_VertexBuffer, SCREEN_CENTER_X + 100 + 333 * 0.5f - 333 * (1.0f - GetVolume()->BGM), SCREEN_CENTER_Y - 50, 40, 40, 0.0f, 0.0f, 1.0f, 1.0f);
	GetDeviceContext()->Draw(4, 0);
}

void DrawVolumeSE(void)
{

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

	// １枚のポリゴンの頂点とテクスチャ座標を設定

	SetSpriteColor(g_VertexBuffer, SCREEN_CENTER_X + 100, SCREEN_CENTER_Y, 333, 30, 0.0f, 0.0f, 1.0f, 1.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);

	// １枚のポリゴンの頂点とテクスチャ座標を設定

	SetSpriteLTColor(g_VertexBuffer, 570.0f, 345, 333 * (GetVolume()->SE), 30, 0.0f, 0.0f, 1.0f * (GetVolume()->SE), 1.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	// ポリゴン描画
	GetDeviceContext()->Draw(4, 0);

	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);
	SetSprite(g_VertexBuffer, SCREEN_CENTER_X + 100 + 333 * 0.5f - 333 * (1.0f - GetVolume()->SE), SCREEN_CENTER_Y, 40, 40, 0.0f, 0.0f, 1.0f, 1.0f);
	GetDeviceContext()->Draw(4, 0);
}

BOOL* GetOptionFlag(void)
{
	return &g_option_flag;
}

void DrawOptions(void)
{	// 頂点バッファ設定
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

	DrawOption();
	DrawVolumeMas();
	DrawVolumeBGM();
	DrawVolumeSE();
	DrawOptionSelect();
	DrawOptionKey();
}

int* GetCursorOption(void)
{
	return &g_option_cursor;
}

