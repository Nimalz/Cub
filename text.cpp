

/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include "text.h"
#include "renderer.h"
#include "FontLoader.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/
#define DW_MAX	(2)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
const WCHAR* FontList[FONT_MAX]
{
	L"メイリオ",
	L"Arial",
	L"Meiryo UI"

};

DIRECTWRITE DirectWrite[DW_MAX];
IDWriteFontFile* pFontFile;
IDWriteFactory* pDWriteFactory;
IDWriteFontCollection* fCollection;
IDWriteTextFormat* pTextFormat;

//=============================================================================
//     初期化
//=============================================================================
HRESULT InitDW(void)
{
	HRESULT hr = S_OK;

	DIRECTWRITE* DW = GetDirectWrite();
	for (int i = 0; i < DW_MAX; i++)
	{
		// フォントデータ
		DW[i].FD = (FONTDATA*)malloc(sizeof(FONTDATA));

		// デフォルト初期化
		DW[i].FD->font = Font::Meiryo;
		DW[i].FD->fontCollection = nullptr;
		DW[i].FD->fontWeight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL;
		DW[i].FD->fontStyle = DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL;
		DW[i].FD->fontStretch = DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL;
		DW[i].FD->fontSize = 20;
		DW[i].FD->localeName = L"ja-jp";
		DW[i].FD->textAlignment = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING;
		DW[i].FD->Color = D2D1::ColorF(D2D1::ColorF::White);

		// Direct2D,DirectWriteの初期化

		// ID2D1Factory インターフェイスの作成
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &DW[i].D2DFactory);
		if (FAILED(hr))
			return hr;
		// DWriteFactory インターフェイスの作成
		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&DW[i].DWriteFactory));
		if (FAILED(hr))
			return hr;
		// バックバッファの取得
		// 型：IDXGISwapChain
		hr = GetSwapChain()->GetBuffer(0, IID_PPV_ARGS(&DW[i].BackBuffer));
		if (FAILED(hr))
			return hr;
		//関数CreateTextFormat()
		//第1引数：フォント名（L"メイリオ", L"Arial", L"Meiryo UI"等）
		//第2引数：フォントコレクション（nullptr）
		//第3引数：フォントの太さ（DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_WEIGHT_BOLD等）
		//第4引数：フォントスタイル（DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STYLE_ITALIC）
		//第5引数：フォントの幅（DWRITE_FONT_STRETCH_NORMAL,DWRITE_FONT_STRETCH_EXTRA_EXPANDED等）
		//第6引数：フォントサイズ（20, 30等）
		//第7引数：ロケール名（L""）
		//第8引数：テキストフォーマット（&g_pTextFormat）
		hr = DW[i].DWriteFactory->CreateTextFormat(
			FontList[(int)DW[i].FD->font],
			DW[i].FD->fontCollection,
			DW[i].FD->fontWeight,
			DW[i].FD->fontStyle,
			DW[i].FD->fontStretch,
			DW[i].FD->fontSize,
			DW[i].FD->localeName,
			&DW[i].TextFormat);
		if (FAILED(hr))
			return hr;

		//関数SetTextAlignment()
		//第1引数：テキストの配置（DWRITE_TEXT_ALIGNMENT_LEADING：前, DWRITE_TEXT_ALIGNMENT_TRAILING：後, DWRITE_TEXT_ALIGNMENT_CENTER：中央,
		//                         DWRITE_TEXT_ALIGNMENT_JUSTIFIED：行いっぱい）
		hr = DW[i].TextFormat->SetTextAlignment(DW[i].FD->textAlignment);
		if (FAILED(hr))
			return hr;



		// dpiの設定
		FLOAT dpiX;
		FLOAT dpiY;
		DW[i].D2DFactory->GetDesktopDpi(&dpiX, &dpiY);

		// レンダーターゲットの作成
		D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), dpiX, dpiY);

		if (!DW[i].RT)
		{
			// サーフェスに描画するレンダーターゲットを作成
			hr = DW[i].D2DFactory->CreateDxgiSurfaceRenderTarget(&DW[i].BackBuffer[0], &props, &DW[i].RT);

			// アンチエイリアシングモード
			DW[i].RT->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

			if (FAILED(hr))
				return hr;

			//ブラシ作成関数
			//第1引数：フォント色（D2D1::ColorF(D2D1::ColorF::Black)：黒, D2D1::ColorF(D2D1::ColorF(0.0f, 0.2f, 0.9f, 1.0f))：RGBA指定）
			hr = DW[i].RT->CreateSolidColorBrush(DW[i].FD->Color, &DW[i].SolidBrush);

			if (FAILED(hr))
				return hr;

		}
		//LoadFont(1);
	}
	return hr;
}

//=============================================================================
//     終了処理
//=============================================================================
void UninitDW(void)
{
	DIRECTWRITE* DW = GetDirectWrite();
	// オブジェクト解放
	for (int i = 0; i < DW_MAX; i++)
	{
		if (DW[i].FD)
		{
			free(DW[i].FD);
			DW[i].FD = NULL;
		}
		if (DW[i].BackBuffer) DW[i].BackBuffer->Release();
		if (DW[i].SolidBrush) DW[i].SolidBrush->Release();
		if (DW[i].RT) DW[i].RT->Release();
		if (DW[i].TextFormat) DW[i].TextFormat->Release();
		if (DW[i].DWriteFactory) DW[i].DWriteFactory->Release();
		if (DW[i].D2DFactory) DW[i].D2DFactory->Release();
		if (DW[i].TextLayout) DW[i].TextLayout->Release();

	}
	if (pFontFile) pFontFile->Release();
	/*if (pFontFace) pFontFace->Release();*/
}



HRESULT SetFont(FONTDATA* ft, int i)
{
	HRESULT hr = S_OK;
	DIRECTWRITE* DW = GetDirectWrite();
	DW[i].FD = ft;
	hr = DW[i].DWriteFactory->CreateTextFormat(
		FontList[(int)DW[i].FD->font],
		DW[i].FD->fontCollection,
		DW[i].FD->fontWeight,
		DW[i].FD->fontStyle,
		DW[i].FD->fontStretch,
		DW[i].FD->fontSize,
		DW[i].FD->localeName,
		&DW[i].TextFormat);
	if (FAILED(hr))
		return hr;

	DW[i].TextFormat->SetTextAlignment(DW[i].FD->textAlignment);
	if (FAILED(hr))
		return hr;

	DW[i].RT->CreateSolidColorBrush(DW[i].FD->Color, &DW[i].SolidBrush);
	if (FAILED(hr))
		return hr;
	return hr;
}

//=============================================================================
// 文字描画
// str：文字列
// pos：描画ポジション
// rect：領域指定
// options：テキストの整形
// index : 使用するDIRECTWRITE構造体配列の添え字
//=============================================================================
void DrawString(WCHAR* str, XMFLOAT2 pos, D2D1_DRAW_TEXT_OPTIONS options, int index)
{
	HRESULT hr = S_OK;
	DIRECTWRITE* DW = GetDirectWrite();

	// ターゲットサイズの取得
	D2D1_SIZE_F TargetSize = DW[index].RT->GetSize();

	// テキストレイアウトを作成
	hr = DW[0].DWriteFactory->CreateTextLayout(str, (UINT32)wcslen(str), DW[index].TextFormat, TargetSize.width, TargetSize.height, &DW[index].TextLayout);

	if (SUCCEEDED(hr)) {
		// 描画位置の確定
		D2D1_POINT_2F points;
		points.x = pos.x;
		points.y = pos.y;

		// 描画の開始
		DW[index].RT->BeginDraw();

		// 描画処理
		DW[index].RT->DrawTextLayout(points, DW[index].TextLayout, DW[index].SolidBrush, options);

		// 描画の終了
		DW[index].RT->EndDraw();
	}
}

void DrawStringRect(WCHAR* str, D2D1_RECT_F rect, D2D1_DRAW_TEXT_OPTIONS options, int index)
{
	DIRECTWRITE* DW = GetDirectWrite();

	// 描画の開始
	DW[index].RT->BeginDraw();

	// 描画処理
	DW[index].RT->DrawText(str, (UINT32)wcslen(str), DW[0].TextFormat, rect, DW[0].SolidBrush, options);

	// 描画の終了
	DW[index].RT->EndDraw();
}

DIRECTWRITE* GetDirectWrite(void)
{
	return &DirectWrite[0];
}

#ifdef _DEBUG
void OutputTest(void)
{
	DIRECTWRITE* DW = GetDirectWrite();
	WCHAR wcText1[] = L"　親譲りの無鉄砲で小供の時から損ばかりしている。小学校に居る時分学校の二階から";
	WCHAR wcText2[] = L"飛び降りて一週間ほど腰を抜かした事がある。なぜそんな無闇をしたと聞く人があるか";
	WCHAR wcText3[] = L"も知れぬ。別段深い理由でもない。新築の二階から首を出していたら、同級生の一人が";
	WCHAR wcText4[] = L"冗談に、いくら威張っても、そこから飛び降りる事は出来まい。弱虫やーい。と囃した";
	WCHAR wcText5[] = L"からである。小使に負ぶさって帰って来た時、おやじが大きな眼をして二階ぐらいから";
	WCHAR wcText6[] = L"飛び降りて腰を抜かす奴があるかと云ったから、この次は抜かさずに飛んで見せますと";
	WCHAR wcText7[] = L"答えた。";

	DW[0].RT->BeginDraw();
	DW[0].RT->DrawText(wcText1, ARRAYSIZE(wcText1) - 1, DW[0].TextFormat, D2D1::RectF(0, 0, 800, 25), DW[0].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
	DW[0].RT->DrawText(wcText2, ARRAYSIZE(wcText2) - 1, DW[0].TextFormat, D2D1::RectF(0, 26, 800, 51), DW[0].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
	DW[0].RT->DrawText(wcText3, ARRAYSIZE(wcText3) - 1, DW[0].TextFormat, D2D1::RectF(0, 52, 800, 77), DW[0].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
	DW[0].RT->DrawText(wcText4, ARRAYSIZE(wcText4) - 1, DW[0].TextFormat, D2D1::RectF(0, 78, 800, 103), DW[0].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
	DW[0].RT->DrawText(wcText5, ARRAYSIZE(wcText5) - 1, DW[0].TextFormat, D2D1::RectF(0, 104, 800, 129), DW[0].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
	DW[0].RT->DrawText(wcText6, ARRAYSIZE(wcText6) - 1, DW[0].TextFormat, D2D1::RectF(0, 131, 800, 155), DW[0].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
	DW[0].RT->DrawText(wcText7, ARRAYSIZE(wcText7) - 1, DW[0].TextFormat, D2D1::RectF(0, 156, 800, 181), DW[0].SolidBrush, D2D1_DRAW_TEXT_OPTIONS_NONE);
	DW[0].RT->EndDraw();
}
#endif

HRESULT LoadFont(int i)
{
	HRESULT hr = S_OK;

	//hr = DirectWrite[i].DWriteFactory->CreateFontFileReference(L"data/Silver.ttf", nullptr, &pFontFile);
	//if (FAILED(hr))
	//	return hr;
	////hr = DirectWrite[i].DWriteFactory->CreateFontFace(DWRITE_FONT_FACE_TYPE_TRUETYPE, 1, &pFontFile, 0, DWRITE_FONT_SIMULATIONS_NONE, &pFontFace);
	////if (FAILED(hr))
	////	return hr;
	//hr = DirectWrite[i].DWriteFactory->CreateTextFormat(L"Silver", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 40, L"ja-jp", &DirectWrite[i].TextFormat);
	//if (FAILED(hr))
	//	return hr;
	//DirectWrite[i].TextFormat->SetTextAlignment(DirectWrite[i].FD->textAlignment);
	//if (FAILED(hr))
	//	return hr;

	//DirectWrite[i].RT->CreateSolidColorBrush(DirectWrite[i].FD->Color, &DirectWrite[i].SolidBrush);
	//if (FAILED(hr))
	//	return hr;
	return hr;
}

void ResetDW(void)
{	DIRECTWRITE* DW = GetDirectWrite();
	for (int i = 0; i < DW_MAX;i++)
	{
		DW[i].FD->font = Font::Meiryo;
		DW[i].FD->fontCollection = nullptr;
		DW[i].FD->fontWeight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL;
		DW[i].FD->fontStyle = DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL;
		DW[i].FD->fontStretch = DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL;
		DW[i].FD->fontSize = 20;
		DW[i].FD->localeName = L"ja-jp";
		DW[i].FD->textAlignment = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING;
		DW[i].FD->Color = D2D1::ColorF(D2D1::ColorF::White);

		DW[i].RT->CreateSolidColorBrush(DW[i].FD->Color, &DW[i].SolidBrush);
		DW[i].DWriteFactory->CreateTextFormat(
			FontList[(int)DW[i].FD->font],
			DW[i].FD->fontCollection,
			DW[i].FD->fontWeight,
			DW[i].FD->fontStyle,
			DW[i].FD->fontStretch,
			DW[i].FD->fontSize,
			DW[i].FD->localeName,
			&DW[i].TextFormat);
	}
}