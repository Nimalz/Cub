

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include "text.h"
#include "renderer.h"
#include "FontLoader.h"

/*******************************************************************************
* �}�N����`
*******************************************************************************/
#define DW_MAX	(2)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
const WCHAR* FontList[FONT_MAX]
{
	L"���C���I",
	L"Arial",
	L"Meiryo UI"

};

DIRECTWRITE DirectWrite[DW_MAX];
IDWriteFontFile* pFontFile;
IDWriteFactory* pDWriteFactory;
IDWriteFontCollection* fCollection;
IDWriteTextFormat* pTextFormat;

//=============================================================================
//     ������
//=============================================================================
HRESULT InitDW(void)
{
	HRESULT hr = S_OK;

	DIRECTWRITE* DW = GetDirectWrite();
	for (int i = 0; i < DW_MAX; i++)
	{
		// �t�H���g�f�[�^
		DW[i].FD = (FONTDATA*)malloc(sizeof(FONTDATA));

		// �f�t�H���g������
		DW[i].FD->font = Font::Meiryo;
		DW[i].FD->fontCollection = nullptr;
		DW[i].FD->fontWeight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL;
		DW[i].FD->fontStyle = DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL;
		DW[i].FD->fontStretch = DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL;
		DW[i].FD->fontSize = 20;
		DW[i].FD->localeName = L"ja-jp";
		DW[i].FD->textAlignment = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING;
		DW[i].FD->Color = D2D1::ColorF(D2D1::ColorF::White);

		// Direct2D,DirectWrite�̏�����

		// ID2D1Factory �C���^�[�t�F�C�X�̍쐬
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &DW[i].D2DFactory);
		if (FAILED(hr))
			return hr;
		// DWriteFactory �C���^�[�t�F�C�X�̍쐬
		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&DW[i].DWriteFactory));
		if (FAILED(hr))
			return hr;
		// �o�b�N�o�b�t�@�̎擾
		// �^�FIDXGISwapChain
		hr = GetSwapChain()->GetBuffer(0, IID_PPV_ARGS(&DW[i].BackBuffer));
		if (FAILED(hr))
			return hr;
		//�֐�CreateTextFormat()
		//��1�����F�t�H���g���iL"���C���I", L"Arial", L"Meiryo UI"���j
		//��2�����F�t�H���g�R���N�V�����inullptr�j
		//��3�����F�t�H���g�̑����iDWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_WEIGHT_BOLD���j
		//��4�����F�t�H���g�X�^�C���iDWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STYLE_ITALIC�j
		//��5�����F�t�H���g�̕��iDWRITE_FONT_STRETCH_NORMAL,DWRITE_FONT_STRETCH_EXTRA_EXPANDED���j
		//��6�����F�t�H���g�T�C�Y�i20, 30���j
		//��7�����F���P�[�����iL""�j
		//��8�����F�e�L�X�g�t�H�[�}�b�g�i&g_pTextFormat�j
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

		//�֐�SetTextAlignment()
		//��1�����F�e�L�X�g�̔z�u�iDWRITE_TEXT_ALIGNMENT_LEADING�F�O, DWRITE_TEXT_ALIGNMENT_TRAILING�F��, DWRITE_TEXT_ALIGNMENT_CENTER�F����,
		//                         DWRITE_TEXT_ALIGNMENT_JUSTIFIED�F�s�����ς��j
		hr = DW[i].TextFormat->SetTextAlignment(DW[i].FD->textAlignment);
		if (FAILED(hr))
			return hr;



		// dpi�̐ݒ�
		FLOAT dpiX;
		FLOAT dpiY;
		DW[i].D2DFactory->GetDesktopDpi(&dpiX, &dpiY);

		// �����_�[�^�[�Q�b�g�̍쐬
		D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), dpiX, dpiY);

		if (!DW[i].RT)
		{
			// �T�[�t�F�X�ɕ`�悷�郌���_�[�^�[�Q�b�g���쐬
			hr = DW[i].D2DFactory->CreateDxgiSurfaceRenderTarget(&DW[i].BackBuffer[0], &props, &DW[i].RT);

			// �A���`�G�C���A�V���O���[�h
			DW[i].RT->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

			if (FAILED(hr))
				return hr;

			//�u���V�쐬�֐�
			//��1�����F�t�H���g�F�iD2D1::ColorF(D2D1::ColorF::Black)�F��, D2D1::ColorF(D2D1::ColorF(0.0f, 0.2f, 0.9f, 1.0f))�FRGBA�w��j
			hr = DW[i].RT->CreateSolidColorBrush(DW[i].FD->Color, &DW[i].SolidBrush);

			if (FAILED(hr))
				return hr;

		}
		//LoadFont(1);
	}
	return hr;
}

//=============================================================================
//     �I������
//=============================================================================
void UninitDW(void)
{
	DIRECTWRITE* DW = GetDirectWrite();
	// �I�u�W�F�N�g���
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
// �����`��
// str�F������
// pos�F�`��|�W�V����
// rect�F�̈�w��
// options�F�e�L�X�g�̐��`
// index : �g�p����DIRECTWRITE�\���̔z��̓Y����
//=============================================================================
void DrawString(WCHAR* str, XMFLOAT2 pos, D2D1_DRAW_TEXT_OPTIONS options, int index)
{
	HRESULT hr = S_OK;
	DIRECTWRITE* DW = GetDirectWrite();

	// �^�[�Q�b�g�T�C�Y�̎擾
	D2D1_SIZE_F TargetSize = DW[index].RT->GetSize();

	// �e�L�X�g���C�A�E�g���쐬
	hr = DW[0].DWriteFactory->CreateTextLayout(str, (UINT32)wcslen(str), DW[index].TextFormat, TargetSize.width, TargetSize.height, &DW[index].TextLayout);

	if (SUCCEEDED(hr)) {
		// �`��ʒu�̊m��
		D2D1_POINT_2F points;
		points.x = pos.x;
		points.y = pos.y;

		// �`��̊J�n
		DW[index].RT->BeginDraw();

		// �`�揈��
		DW[index].RT->DrawTextLayout(points, DW[index].TextLayout, DW[index].SolidBrush, options);

		// �`��̏I��
		DW[index].RT->EndDraw();
	}
}

void DrawStringRect(WCHAR* str, D2D1_RECT_F rect, D2D1_DRAW_TEXT_OPTIONS options, int index)
{
	DIRECTWRITE* DW = GetDirectWrite();

	// �`��̊J�n
	DW[index].RT->BeginDraw();

	// �`�揈��
	DW[index].RT->DrawText(str, (UINT32)wcslen(str), DW[0].TextFormat, rect, DW[0].SolidBrush, options);

	// �`��̏I��
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
	WCHAR wcText1[] = L"�@�e����̖��S�C�ŏ����̎����瑹�΂��肵�Ă���B���w�Z�ɋ��鎞���w�Z�̓�K����";
	WCHAR wcText2[] = L"��э~��Ĉ�T�ԂقǍ��𔲂�������������B�Ȃ�����Ȗ��ł������ƕ����l�����邩";
	WCHAR wcText3[] = L"���m��ʁB�ʒi�[�����R�ł��Ȃ��B�V�z�̓�K�������o���Ă�����A�������̈�l��";
	WCHAR wcText4[] = L"��k�ɁA������В����Ă��A���������э~��鎖�͏o���܂��B�㒎��[���B�ƚ�����";
	WCHAR wcText5[] = L"����ł���B���g�ɕ��Ԃ����ċA���ė������A���₶���傫�Ȋ�����ē�K���炢����";
	WCHAR wcText6[] = L"��э~��č��𔲂����z�����邩�Ɖ]��������A���̎��͔��������ɔ��Ō����܂���";
	WCHAR wcText7[] = L"�������B";

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