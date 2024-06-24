

#pragma once

#pragma warning(push)
#pragma warning(disable:4005)

#include "main.h"

#include <d2d1.h>
#include <dwrite.h>

//*****************************************************************************
// ���C�u�����̃����N
//*****************************************************************************
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

//=============================================================================
//		�t�H���g���X�g
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
	Font font;								// �t�H���g��
	IDWriteFontCollection* fontCollection;	// �t�H���g�R���N�V����
	DWRITE_FONT_WEIGHT fontWeight;			// �t�H���g�̑���
	DWRITE_FONT_STYLE fontStyle;			// �t�H���g�X�^�C��
	DWRITE_FONT_STRETCH fontStretch;		// �t�H���g�̕�
	FLOAT fontSize;							// �t�H���g�T�C�Y
	WCHAR const* localeName;				// ���P�[����
	DWRITE_TEXT_ALIGNMENT textAlignment;	// �e�L�X�g�̔z�u
	D2D1::ColorF Color;						// �t�H���g�̐F

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

	// �t�H���g�f�[�^
	FONTDATA* FD;

};


/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/

HRESULT InitDW(void);		// DIRECT_WRITE�̏���������
void UninitDW(void);		// DIRECT_WRITE�̏I������

// �����`�揈��
// WCHAR�F������
// pos�F�`��|�W�V����
// options�F�e�L�X�g�̐��`
void DrawString(WCHAR* FontList, XMFLOAT2 pos, D2D1_DRAW_TEXT_OPTIONS options, int index);

// �����`�揈��
// string�F������
// rect�F�̈�w��
// options�F�e�L�X�g�̐��`
void DrawStringRect(WCHAR* FontList, D2D1_RECT_F rect, D2D1_DRAW_TEXT_OPTIONS options, int index);


// �t�H���g�ݒ�
// ��1�����F�t�H���g�f�[�^�\����
// ��2�����F�ύX����DIRECTWRITE�\���̔z��̓Y����
HRESULT SetFont(FONTDATA* ft, int i);
HRESULT LoadFont(int);

DIRECTWRITE* GetDirectWrite(void);
void ResetDW(void);

#ifdef _DEBUG
void OutputTest(void);
#endif