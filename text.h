

#pragma once

#pragma warning(push)
#pragma warning(disable:4005)

#include "main.h"

#include <d2d1.h>
#include <dwrite.h>

//*****************************************************************************
// ライブラリのリンク
//*****************************************************************************
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

//=============================================================================
//		フォントリスト
//=============================================================================
enum Font
{
	Meiryo,
	Arial,
	MeiryoUI,

	FONT_MAX
};

struct FONTDATA
{
	Font font;								// フォント名
	IDWriteFontCollection* fontCollection;	// フォントコレクション
	DWRITE_FONT_WEIGHT fontWeight;			// フォントの太さ
	DWRITE_FONT_STYLE fontStyle;			// フォントスタイル
	DWRITE_FONT_STRETCH fontStretch;		// フォントの幅
	FLOAT fontSize;							// フォントサイズ
	WCHAR const* localeName;				// ロケール名
	DWRITE_TEXT_ALIGNMENT textAlignment;	// テキストの配置
	D2D1::ColorF Color;						// フォントの色

};

struct DIRECTWRITE
{
	ID2D1Factory* D2DFactory;
	IDWriteFactory* DWriteFactory;
	IDWriteTextFormat* TextFormat;
	IDWriteTextLayout* TextLayout;
	ID2D1RenderTarget* RT;
	ID2D1SolidColorBrush* SolidBrush;
	IDXGISurface* BackBuffer;

	// フォントデータ
	FONTDATA* FD;

};


/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/

HRESULT InitDW(void);		// DIRECT_WRITEの初期化処理
void UninitDW(void);		// DIRECT_WRITEの終了処理

// 文字描画処理
// WCHAR：文字列
// pos：描画ポジション
// options：テキストの整形
void DrawString(WCHAR* FontList, XMFLOAT2 pos, D2D1_DRAW_TEXT_OPTIONS options, int index);

// 文字描画処理
// string：文字列
// rect：領域指定
// options：テキストの整形
void DrawStringRect(WCHAR* FontList, D2D1_RECT_F rect, D2D1_DRAW_TEXT_OPTIONS options, int index);


// フォント設定
// 第1引数：フォントデータ構造体
// 第2引数：変更するDIRECTWRITE構造体配列の添え字
HRESULT SetFont(FONTDATA* ft, int i);
HRESULT LoadFont(int);

DIRECTWRITE* GetDirectWrite(void);
void ResetDW(void);

#ifdef _DEBUG
void OutputTest(void);
#endif