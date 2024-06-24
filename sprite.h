#pragma once
#include "bg.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PI  3.14159f



//アイソメトリックテクスチャの向き


//*****************************************************************************
// 構造体定義 
//*****************************************************************************

struct SPRITE
{
	XMFLOAT3	pos;			// ポリゴンの座標
	XMFLOAT3	rot;			// ポリゴンの回転量
	BOOL		use;			// true:使っている  false:未使用
	float		w, h;			// 幅と高さ
	int         facing;			// 顔向き

	int			countAnim;		// アニメーションカウント
	int			patternAnim;	// アニメーションパターンナンバー
	int			texNo;			// テクスチャ番号
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void SetSprite(ID3D11Buffer* buf, float X, float Y, float Width, float Height, float U, float V, float UW, float VH);

void SetSpriteColor(ID3D11Buffer* buf, float X, float Y, float Width, float Height, float U, float V, float UW, float VH, XMFLOAT4 color);

void SetSpriteColorRotation(ID3D11Buffer* buf, float X, float Y, float Width, float Height,
	float U, float V, float UW, float VH,
	XMFLOAT4 Color, float Rot);

void SetSpriteColorRotationBottomUp(ID3D11Buffer* buf, float X, float Y, float Width, float Height,
	float U, float V, float UW, float VH,
	XMFLOAT4 Color, float Rot);

void SetSpriteColorRotIso(ID3D11Buffer* buf, float X, float Y, float Width, float Height,
	float U, float V, float UW, float VH,
	XMFLOAT4 Color, float Rot,
	float ratio);

//void SetSpriteColorRotIsoAbyBandDraw(ID3D11Buffer* buf, float X, float Y, float Width, float Height,
//	float U, float V, float UW, float VH,
//	XMFLOAT4 Color, float Rot,
//	float ratio,
//	ID3D11ShaderResourceView** Texture,
//	int MappChipList[MAP_SIZE_WIDTH][MAP_SIZE_HEIGHT]);

void SetSpriteIso(ID3D11Buffer* buf, float X, float Y, float TexSize, float Polisize,
	float U, float V, float UW, float VH,
	ID3D11ShaderResourceView** Texture,
	float* MappChipList,
	int height, int width);

void SetSpriteIsoAndDrawLayer2(ID3D11Buffer* buf, float X, float Y, float TexSize, float Polisize,
	float U, float V, float UW, float VH,
	ID3D11ShaderResourceView** Texture,
	float* MappChipList,
	int height, int width);

void SetSpriteLeftTop(ID3D11Buffer* buf, float X, float Y, float Width, float Height, float U, float V, float UW, float VH);

void SetSpriteLTColor(ID3D11Buffer* buf,
	float X, float Y, float Width, float Height,
	float U, float V, float UW, float VH,
	XMFLOAT4 color);

void SetSpriteWithVertexsColor(ID3D11Buffer* buf, XMFLOAT3 v1, XMFLOAT3 v2, XMFLOAT3 v3, XMFLOAT3 v4,
	float U, float V, float UW, float VH,
	XMFLOAT4 Color);



void SetSpriteColorPerspetiveFloorandDraw(ID3D11Buffer* buf, float X, float Y, float Width, float Height,
	float U, float V, float UW, float VH,
	XMFLOAT4 Color,
	float s1, float s2, float h);

void SetSpriteColorOrthoRot(ID3D11Buffer* buf, XMFLOAT3 v1, XMFLOAT3 v2, XMFLOAT3 v3, XMFLOAT3 v4,
	float U, float V, float UW, float VH,
	XMFLOAT4 Color,
	XMFLOAT3 Campos,
	XMFLOAT3 Camat);

XMVECTOR OrthoProJection(XMFLOAT3 Vertexin, XMFLOAT3 campos, XMFLOAT3 camat);

XMVECTOR LinearInterpolation(XMVECTOR p0, XMVECTOR  p1, float s);